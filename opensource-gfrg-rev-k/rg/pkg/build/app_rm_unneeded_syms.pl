######################################################################
#                                                                    #
#                                                                    #
#     purpose: for each exe(openrg,asterisks,.....)                  #
# this program  intersect between                                    #
# 1. the undefind syms in the rg_gcc input objs (*.o,*.a)            #
# 2. exe defined syms                                                #
# 3. input *.so defined syms                                         #
# add this to needed syms for mklibs (because each such defined      #
# symbol mask form mklibs the need to retain that symbol)            # 
# also as by product try to strip the symbol from the object         #
# all this was done because for combinations of undefined symbols    #
# in one of the (*.o,*.a) and defined sym in *.so                    #
# gcc creat symbol with the same name in                             #
# the resulting  exe                                                 #
######################################################################
use strict;
use POSIX;
use File::Copy;
use Env qw(JMK_ROOT);
use lib qq{$JMK_ROOT/pkg/build};
use rg_utils;

my ($rg_root,$app,$app_file_name,$app_prefix,@cmd)=@ARGV;
my $cmd = join " ",@cmd;

my $rg_utils = new rg_utils($rg_root);
my $build=qq{$rg_root/build};
my $f1=qq{$build/pkg/build/$app_prefix.f1};
my $f2=qq{$build/pkg/build/$app_prefix.f2};

my $strip=$rg_utils->get_binutil_path($build,'strip');
my $nm=$rg_utils->get_binutil_path($build,'nm');
my $objcopy=$rg_utils->get_binutil_path($build,'objcopy');
my (%objs,@objs);
my %so_libs;

my (%app_defined_syms,%objs_defined_syms,%objs_undefined_syms,%libs_so_defined_syms);
my @needed_syms;
my @lines = `$cmd 2>&1`;
if ($?)
{
  my $lines = join "\n",@lines;
  die qq{$lines\n failed to compile $app\n}; 
}
foreach  (@lines)
{
  (split)>2 && print && next;	
  chomp;
  (-e $_) && ($objs{$_}=1) && next;
  /^\(([^)]+\.a)\)/ && (-e $1) && ($objs{$1}=1) && next;
  /^\-\S+\s+\(([^)]+\.so)/  && (-e $1)  && ($so_libs{$1}=1);
}

close CMD;

@objs = keys %objs;
get_syms($app,"mklibs-readelf","-p",\%app_defined_syms,0);

foreach my $obj (@objs)
{
  get_syms($obj,$nm,'--undefined-only',\%objs_undefined_syms,-1);
  get_syms($obj,$nm,"--defined-only",\%objs_defined_syms,-1);
}

foreach my $so (keys %so_libs)
{
  get_syms($so,"mklibs-readelf","-p",\%libs_so_defined_syms,0);
}

@needed_syms = get_needed_list();
mkdir qq{$build/mklibs/};
open SYMS,qq{>>$build/mklibs/needed_syms.lst};
foreach my $sym (@needed_syms)
{
  print SYMS qq{$sym $app_file_name\n};
}

close SYMS;

clean_app();

1;

sub copy_obj
{
  my ($src,$dst,%syms)=@_;
  my %invert_syms;
#  map {$invert_syms{$syms{$_}}=$_} keys %syms;

  my $command = qq{$objcopy -N } . join( qq{ -N $_},keys %syms) . qq{ $src $dst};
  my $output=`$command 2>&1`;
  return $2 if ($output=~/(\.\w+\.)?(\w+)[^\w.]+required but not present/);
  return undef;
}

sub clean_app
{
  copy $app,$f1;
  my $change=1;
  my %syms=map {$_=>1} @needed_syms;
  while(defined $change && scalar keys  %syms)
  {
    undef $change; 
    my %s =   %syms;
      
    while (keys(%s) && !defined $change)
    {
      if (!defined(my $sym= copy_obj($f1,$f2,%s)))
      {
        copy $f2,$f1 ;
        $change=1;
        delete @syms{keys %s};
      }
      else
      {
        delete $s{$sym};
      }
    }
 }
 copy $f1,$app if (!scalar keys %syms);
}

sub get_needed_list
{
  my @syms;	

  foreach my $sym (keys %app_defined_syms)
  {
    next if (!exists $objs_undefined_syms{$sym}  ||
	    exists $objs_defined_syms{$sym} ||
	    !exists $libs_so_defined_syms{$sym});	 

    push @syms,$sym;
  }
  return @syms;
}

sub get_syms
{
  my ($elf,$elf_reader,$filter,$table,$index)=@_;

  open CMD,qq{$elf_reader $filter $elf|};

  while(<CMD>)
  {
    $table->{(split)[$index]}{$elf}=1;
  }
  close CMD;
}



