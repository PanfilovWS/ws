import subprocess

subprocess.call('find . -type d -maxdepth 1 -mindepth 1| xargs -i tar -cf {}.tar {}| mkdir archive| scp -r *.tar.gz', shell=True)
subprocess.call('chown serost:serost *', shell=True)
subprocess.call('scp -r archive pi@192.168.1.97:/home/pi', shell=True)
subprocess.call('rm -rfv archive| rm -rfv *.tar', shell=True)
