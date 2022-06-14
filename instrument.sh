( cd $(dirname $0); make )
LD_PRELOAD=$(dirname $0)/preload.so $@
