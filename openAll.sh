unameOut="$(uname -s)"
case "${unameOut}" in
    Linux*)     xdg-open *.c* *.h* */*.c* */*.h*;;
    Darwin*)    open *.c* *.h* */*.c* */*.h*;;
    CYGWIN*)    cygstart *.c* *.h* */*.c* */*.h*;;
    MINGW*)     start "" *.c* *.h* */*.c* */*.h*;;
    MSYS_NT*)   start "" *.c* *.h* */*.c* */*.h*;;
    *)          echo "Unknown machine name \"${unameOut}\"; you might have to open all the files yourself."
                xdg-open *.c* *.h* */*.c* */*.h*
esac
