#!/bin/sh
# ÿ�δ�SVNȡ����ʱ����Ҫ���иýű���֮����δ��SVNȡ���ļ�֮ǰ����Ҫ������
# author: eyjian@qq.com or eyjian@gmail.com

# basedirΪԴ�����Ÿ�Ŀ¼
basedir=.
cd $basedir


############################
# ɾ�����е�.svnĿ¼���ļ�
############################
find $basedir -name .svn | xargs rm -fr


############################
# ����Ϊ����Makefile.am�ļ�
############################

gen_makefile_am()
{
	old=$sub/Makefile.am.in
	new=$sub/Makefile.am

	if test ! -f $old; then
		return;
	fi

	if test -f $new; then
		rm $new;
	fi

	# Get all cpp files in $sub directory
	cd $sub
	cpp_files=`ls *|awk '/.cpp|.cc/{printf("%s ", $0)}'`
	cd - > /dev/null 2>&1

	# Remove carriage return, and append CPP after the line included _SOURCES
	awk -v files="$cpp_files" '{ gsub("\r",""); if (match($0,"_SOURCES")) printf("%s %s\n",$0,files); else printf("%s\n",$0); }' $old > $new
}

rec_subdir()
{
	if test $# -ne 1; then
		echo "Parameter error in rec_subdir"
		exit
	fi

	subdirs=`find $1 -type d`
	for sub in $subdirs
	do
		# Skip the current and parent directory
		if test $sub = "." -o $sub = ".."; then
			continue;
		fi

		gen_makefile_am $sub
	done
}

rec_subdir $basedir



############################
# ����Ϊ����configure.ac�ļ�
############################

# ��дconfigure.ac�е�autoconf�汾��
replace_autoconf_version()
{
	autoconf_version=`autoconf --version|head -n1|cut -d' ' -f4`
	sed 's/AUTOCONF_VERSION/'$autoconf_version'/' configure.ac.in > configure.ac
}

# ����Make.rules�ļ�
check_make_rules()
{
	bit=`getconf LONG_BIT`
	if test $bit -eq 64; then
		sed 's/^MY_CXXFLAGS/#MY_CXXFLAGS/' Make.rules.in > Make.rules
	else
		cp Make.rules.in Make.rules
	fi
}

# ���ļ���ʽ��DOSת����UNIX
d2x()
{
    for file in $*
    do
            src_file=$file
            tmp_file=$file.tmp

	    if test -d $src_file; then
            continue
	    fi

            tr -d "\r" < $src_file > $tmp_file
            if test $? -eq 0; then
                    mv $tmp_file $src_file
                    echo "Convert $src_file from the format of DOS to UNIX OK."
            fi
    done
}

# ��ҪDOS��ʽת�����ļ�
d2x ltmain.sh
d2x configure.ac.in
d2x Makefile.am
d2x Make.rules.in

check_make_rules
replace_autoconf_version


##########################################
# ����Ϊ����configure�ļ���Makefile.in�ļ�
##########################################

aclocal
if test $? -ne 0; then
	echo "aclocal ERROR"
	exit
fi
autoconf
if test $? -ne 0; then
	echo "autoconf ERROR"
	exit
fi
autoheader
if test $? -ne 0; then
	echo "autoheader ERROR"
	exit
fi
automake -a
if test $? -ne 0; then
	echo "automake -a ERROR"
	exit
fi

chmod +x *.sh
chmod +x configure


#################################################
# �������Ϳ��Կ�ʼִ��configure����Makefile�ļ���
#################################################
