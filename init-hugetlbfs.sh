#!/bin/bash

#####################################################################
#
# Huge TLB Initialization Script
#
# Description:
#
# This script mounts hugetlbfs which enables the usage of huge-pages
# via mmap syscall interface in linux. It is essential that this
# script should be executed once before using the huge-page malloc
# allocator
#
#####################################################################

mount | grep hugetlbfs || mount -t hugetlbfs none /mnt/huge
echo 1024 > /proc/sys/vm/nr_hugepages

