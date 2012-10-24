######################################################################
#                                                                    #
#this script         do 3 main things                                #
# 1.for each .so found at  cramfs_dir/libthat lack the               #
# suitable _pic.a  look for the suitable .a and link it              #
# under build/debug/*_pic.a                                          #
#                                                                    #
# 2. run mklibs                                                      #
#                                                                    #
# 3.copy mklibs so back to cramfs_dir/lib                            #          #                                                                    #
#                                                                    #
######################################################################
use strict;
use POSIX;
use File::Find;
use File::Basename;
use File::Copy;
use FindBin;
use Env qw(JMK_ROOT);
use lib qq{$JMK_ROOT/pkg/build};
use rg_utils;

my $rg_root=$JMK_ROOT;
my $rg_utils = new rg_utils($rg_root);
my $build_var = $ENV{"BUILD"};
if ($build_var eq "") { $build_var = "build"; }
my $build=qq{$rg_root/$build_var};
my $cramfs_lib_dir = qq{$build/pkg/build/disk_image/cramfs_dir/lib/};
my $ramdisk_lib_dir = qq{$build/pkg/build/disk_image/ramdisk_dir/lib/};
#take the prefix for each dir
my $mklib_root_dir = qq{$build/mklibs/};
my (%so_ramfs2cramfs);

chdir $rg_root;
prepare_for_mklibs();
find(\&wanted, qq{$build/pkg});
run_mklibs();
mklibs2cramfs();

sub mklibs2cramfs
{

  #after running mklibs put results in cramfs

  foreach my $lib (<$mklib_root_dir/dist/lib*so*>)
   {

    my $bases = basename($lib);
    die qq{cant find orig for  $bases\n} if (!exists $so_ramfs2cramfs{$bases} &&
                                           !-e qq{$cramfs_lib_dir/$bases});
    my $baset = exists $so_ramfs2cramfs{$bases} ? $so_ramfs2cramfs{$bases} : $bases;
    my $tlib = qq{$cramfs_lib_dir/$baset};
    next if (-s $lib >= -s $tlib);
    copy $lib,$tlib;
   }
}

sub prepare_for_mklibs
{
 mkdir $mklib_root_dir if (!-d $mklib_root_dir);
 mkdir qq{$mklib_root_dir/dist} if (!-d qq{$mklib_root_dir/dist});
 mkdir qq{$mklib_root_dir/lib} if (!-d qq{$mklib_root_dir/libs});
#copy so which have different names  as -L input for mklibs
  unlink <$mklib_root_dir/dist/*>;
  unlink <$mklib_root_dir/lib/*>;

 foreach my $link (<$ramdisk_lib_dir/*>)
  {
    next if (!-l $link);
    my $target = readlink $link;
    my $basel =  basename($link);
    my $baset = basename($target);
    next if ($baset eq $basel);
    next if (-e qq{$build/pkg/build/disk_image/cramfs_dir/lib/$basel});
       die qq{$build/pkg/build/disk_image/cramfs_dir/lib/$baset doesnt exists\n}
    if (! -e qq{$build/pkg/build/disk_image/cramfs_dir/lib/$baset});
    copy qq{$build/pkg/build/disk_image/cramfs_dir/lib/$baset},qq{$mklib_root_dir/lib/$basel};
   $so_ramfs2cramfs{$basel} = $baset;
   $basel=~s/(\.so)(\.\d)?$/$1/;
    copy qq{$build/pkg/build/disk_image/cramfs_dir/lib/$baset},qq{$mklib_root_dir/lib/$basel};
  }

}

sub run_mklibs
{
  my $objcopy=$rg_utils->get_binutil_path($build,'objcopy');
  my $prefix = $objcopy;
  $prefix=~ s/.objcopy$//;
  my $toolchain_lib = dirname(dirname($objcopy)). q{/lib};
  my $command = qq{
python $rg_root/pkg/build/mklibs -D -v --target $prefix --ldlib ${prefix}-ld   -L $build/pkg/build/disk_image/cramfs_dir/lib  -L $build/debug -L $mklib_root_dir/lib -d $mklib_root_dir/dist $build/pkg/build/disk_image/cramfs_dir/bin/* 2>&1

  };
  print $command;
  print `$command`;
}

sub wanted
  {
#create _pic.a from .a libs for so's which the make itself doesnt create

    return if ( !/(.+)(_pic)?\.a$/);
    my $l = $1;
    $l=~s/_so$//;
    $l=~s/([+*])/\\$1/g;
    my ($lib) = grep /^$l\./, keys %so_ramfs2cramfs ;
    return if (!defined $lib);
    $lib=~s/\.so.*$//;
    my $pic_name = qq{${lib}_pic.a};
    
    return  if ((-e qq{$build/debug/$pic_name}) && !/_so\.a$/);
    unlink qq{$build/debug/$pic_name};

    symlink $File::Find::name,qq{$build/debug/$pic_name};
  }

