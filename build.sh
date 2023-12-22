
isTest=false

if [ "$1" = "-t" ] || [ "$2" = "-t" ] || [ "$3" = "-t" ]; then
	isTest=true
fi

isValgrind=false
if [ "$1" = "-mem" ] || [ "$2" = "-mem" ] || [ "$3" = "-mem" ] ; then
	isValgrind=true
fi

isCuda=false
if [ "$1" = "-g" ] || [ "$2" = "-g" ] || [ "$3" = "-g" ] ; then
	isCuda=true
fi

appendPrefix() {
	local array_name="$1[@]"
	local input_array=("${!array_name}")
	local prefix="$2"
	local new_array=()

	for element in "${input_array[@]}"; do
		new_array+=("${prefix}${element}")
	done

	echo "${new_array[@]}"
}

# ------------------- EXTERNAL TPDBv2 ------------------- #
externTPDB="./external/TPDBv2/src/"

TPDBv2UtilsPath="./external/TPDBv2/src/Utils/"
TPDBv2UtilsC=("StringTools.c" "MiscTools.c" "SerializeTools.c")
TPDBv2UtilsRelative=$(appendPrefix "TPDBv2UtilsC" "${TPDBv2UtilsPath}" "${TPDBv2UtilsC}")

TPDBv2ErrorsPath="./external/TPDBv2/src/Errors/"
TPDBv2ErrorsC=("I_Errors.c")
TPDBv2ErrorsRelative=$(appendPrefix "TPDBv2ErrorsC" "${TPDBv2ErrorsPath}" "${TPDBv2ErrorsC}")

TPDBv2StoragePath="./external/TPDBv2/src/Storage/"
TPDBv2StorageC=("Storage.c")
TPDBv2StorageRelative=$(appendPrefix "TPDBv2StorageC" "${TPDBv2StoragePath}" "${TPDBv2StorageC}")

TPDBv2InterfacePath="./external/TPDBv2/src/Interface/"
TPDBv2InterfaceC=("I_Database.c" "I_Row.c" "I_Table.c" "I_Indexing.c")
TPDBv2InterfaceRelative=$(appendPrefix "TPDBv2InterfaceC" "${TPDBv2InterfacePath}" "${TPDBv2InterfaceC}")

completeCPathsTPDBv2=($TPDBv2UtilsRelative $TPDBv2ErrorsRelative $TPDBv2StorageRelative $TPDBv2InterfaceRelative)
# ------------------- EXTERNAL TPDBv2 ------------------- #

# ------------------- GPU ACCELERATED ------------------- #
TPCrossGPU_Path="./src/GpuAccelerated/"
TPCrossGPU_C=("Cross_Cuda_Utils" "Cross_Cuda" "I_Cross_Cuda")
TPCrossGPU_Relative=$(appendPrefix "TPCrossGPU_C" "${TPCrossGPU_Path}" "${TPCrossGPU_C}")

completeCNamesTpCrossGPU=("${TPCrossGPU_C[@]}")
completeCPathsTpCrossGPU=($TPCrossGPU_Relative)
TPCrossGPUObj_Complete=""

for ((i=0; i<"${#completeCNamesTpCrossGPU[@]}"; i++)); do
	temp="./bin/CROSS_CUDA/TPCrossGpu_${completeCNamesTpCrossGPU[i]}.o"
	nvcc -arch=sm_86 -g -o "${temp}" -c "${completeCPathsTpCrossGPU[i]}.cu"
	TPCrossGPUObj_Complete+="${temp} "
done

ld -r -o ./bin/TPCrossGpu.o ./bin/CROSS_CUDA/TPCrossGpu_*.o

# ------------------- GPU ACCELERATED ------------------- #

TPCrossDatabasePath="./src/Database/"
TPCrossDatabaseC=("DatabaseManager.c")
TPCrossDatabaseRelative=$(appendPrefix "TPCrossDatabaseC" "${TPCrossDatabasePath}" "${TPCrossDatabaseC}")

TPCrossUtilsPath="./src/Utils/"
TPCrossUtilsC=("MiscTools.c" "CrossSerializers.c")
TPCrossUtilsRelative=$(appendPrefix "TPCrossUtilsC" "${TPCrossUtilsPath}" "${TPCrossUtilsC}")

completeCPathsTPCross=("./src/Errors/I_Cross_Errors.c" $TPCrossUtilsRelative $TPCrossDatabaseRelative)

projectRunC="./src/"
runPath="./build/"

if [ $isTest = true ]; then
	echo "Building Unit tests executable"
	projectRunC+="Tests/UnitTests.c"
	runPath+="test"
else
	echo "Building executable"
	projectRunC+="main.c"
	runPath+="run"
fi

gcc $projectRunC "${completeCPathsTPDBv2[@]}" "${completeCPathsTPCross[@]}" ./bin/TPCrossGpu.o -L/usr/local/cuda/lib64 -lcudart -o $runPath -lm -g

if [ $isValgrind = true ]; then
	if [ $isCuda = true ]; then
		compute-sanitizer --tool memcheck $runPath
	else
		valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --read-inline-info=yes -s $runPath
	fi
else
	$runPath
fi
