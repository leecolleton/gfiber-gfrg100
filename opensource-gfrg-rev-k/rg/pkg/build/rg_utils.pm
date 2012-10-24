
package rg_utils;

sub new
{
  my ($class,$rg_root)=@_;
  die "you must supply rg root\n" if (!-e qq{$rg_root/jmk/rg_root.mak});
  my $self = {
	       rg_root=>$rg_root
             };
  return bless $self;
}

sub get_build_var
{
  # @_ is the input array - extract the 3 input parameters
  my ($self,$build,$var)=@_;	
  # open the file rg_config.mk (CONFIG 'points' to the next line in the file)
  open CONFIG,qq{$build/rg_config.mk};
  my $value;
  # loop over all lines in the file
  while(<CONFIG>)
  {
    # if the CONFIG line begins with 'export' followed by at lease one space
    # and then some xxx=yyy format (or the line just begins with xxx=yyy)
    if (/^(export\s+)?$var=(\S+)/)
    { 
      # $value will hold the yyy from above format
      $value=$2;
      # make this the last iteration
      last;
    }
  }
  close CONFIG;
  return $value;
}

sub get_binutil_path
{
  # @_ is the input array - extract the 3 input parameters
  my ($self,$build,$binutil)=@_;
  # find in rg_config.mk the value that followes: 'export TOOLS_PREFIX='
  my $tools_prefix = $self->get_build_var($build,'TOOLS_PREFIX');
  # make an array from all the files that match this format:
  my ($util) = <$tools_prefix/bin/*$binutil*>;
  return $util;
}

1;
