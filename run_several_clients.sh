build_type=$1
client_count=$2

if [ ${build_type} != "Debug" ] && [ ${build_type} != "Release" ];
then
    echo "Argument 1 bust be \"Debug\" or \"Release\""
    exit 1
fi

for i in $(seq 1 ${client_count});
do
    ./build/${build_type}/client/client &
done

