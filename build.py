import time
import argparse
import platform
import os
import colorama


class Builder:
    info = colorama.Fore.BLUE
    ok = colorama.Fore.GREEN
    warning = colorama.Fore.LIGHTRED_EX
    critical = colorama.Fore.RED
    reset = colorama.Style.RESET_ALL
    system = platform.system()
    release = platform.release()
    vs32 = '"Visual Studio 15 2017"'
    vs64 = '"Visual Studio 16 2019"'
    ninja = 'Ninja'
    usage = 'sudo python3 install.py -b ninja|vs32|vs64 -p 8'
    descript = '''The number of threads in the -p flag is indicated from\
    the calculation: the number of processor cores * 2 + 1'''
    
    def check_depends(self):
        print(Builder.info, 'Install Dependent Libraries', Builder.reset)
        time.sleep(3)
        if 'MAJARO' or 'ARCH' in Builder.release:
            os.system('pacman -S --needed cmake gcc ninja openssl')
        elif 'Ubuntu' in Builder.release:
            os.system('apt-get install build-essential autoconf libtool pkg-config libgflags-dev\
            libgtest-dev clang libc++-dev -y')
        elif 'Windows' in Builder.system:
            os.environ['CC']='cl.exe'
            os.environ['CXX']='cl.exe'
        else:
            print(Builder.critical, 'ERROR! Platform not supported!', Builder.reset)

    def building(self, args):
        if os.path.isdir(r"build") != True:
            os.mkdir(r"build")
        os.chdir('./build')
        if args.build[0] == 'ninja':
            os.system(f'cmake .. -G {Builder.ninja}')
        elif args.build[0] == 'vs32':
            os.system(f'cmake .. -G {Builder.vs32}')
        elif args.build[0] == 'vs64':
            os.system(f'cmake .. -G {Builder.vs64}')
        os.system(f'cmake --build . --config Release --parallel {args.parallel[0]}')

    def parse_args(self):
        parser = argparse.ArgumentParser(usage=Builder.usage, description=Builder.descript)
        parser.add_argument('-b', '--build', nargs=1, required=True, choices=['ninja', 'vs32', 'vs64'])
        parser.add_argument('-p', '--parallel', nargs=1, default='2')
        return parser.parse_args()


if __name__ == '__main__':
    install = Builder()
    args = install.parse_args()
    install.check_depends()
    install.building(args)




