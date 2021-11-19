build_type=$1
client_count=$2

if [ ${build_type} != "debug" ] && [ ${build_type} != "release" ];
then
    echo "Argument 1 bust be \"debug\" or \"release\""
    exit 1
fi

for i in $(seq 1 ${client_count});
do
    ./build/${build_type}/client/client &
done

