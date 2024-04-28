cmake -B GDExtensionTemplate-build -DCMAKE_BUILD_TYPE=${1:-Debug} -DCMAKE_INSTALL_PREFIX=GDExtensionTemplate-install GDExtensionTemplate
cmake --build GDExtensionTemplate-build
cmake --install GDExtensionTemplate-build
