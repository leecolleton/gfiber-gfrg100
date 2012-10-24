#!/bin/bash

make_node_rm()
{
    dst_dir=${1%/*}
    rm -f $1
    mkdir -p $dst_dir
    $JMKE_BUILDDIR/pkg/build/exec_as_root mknod $@
}

panic()
{
    echo "Panic: $1" 1>&2
    exit 1
}

rg_vpath_cp()
{
    src=$1
    dst=$2
    dst_dir=${dst%/*}
    
    if [ ! -d $dst_dir ] ; then
        echo "mkdir -p $dst_dir"
        mkdir -p $dst_dir || return 1
    fi
    src_path=$src

    if [ ! -z "$DO_LINK" ] && [ "$src_path" == "$dst" ] ; then
	#echo $0: $dst already linked
	return 0
    fi

    if [ -e $dst ] ; then
        rm $dst || return 1
    fi

    if [ -n "$DO_LINK" ]; then 
        rg_lnf $src_path $dst || return 1
    else
        $JMKE_BUILDDIR/pkg/build/export_src $src_path $dst
    fi
}

rg_lnf()
{
    if [ -e $1 ] ; then
        ln -sfn $1 $2
    fi
}

cpy_imgs()
{
  dir=$1
  root=$2
  
  # Copy common CSS images
  cp -a $root/pkg/doc/scripts/html_theme.css $dir
  cp -a $root/pkg/doc/scripts/images/*.gif $dir/images
  cp -a $root/pkg/doc/scripts/images/*.png $dir/images
  cp -a /usr/share/xml/docbook/stylesheet/nwalsh/images/draft.png $dir/images
  
  # Copy all images avilabe in the html files to the local HTML image
  # directory. 
  # Search for the images in the HTML files and copy it if exsist (can
  # be CSS images as well)
  for f in `ls $dir/*.html` ; do
    images=`cat $f | \
      perl -ne 'map {print qq{$_\n}} /src="images.(\w+\.\w+)/g'`;
    for cpfile in $images ; do 
      if [ -f images/$cpfile ] ; then 
        cp images/$cpfile $dir/images; 
      fi 
    done 
  done
}

