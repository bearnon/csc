#!/usr/bin/python3


from scapy.all import ARP,Ether,srp
import netifaces

def main():
    # request = Ether(dst='ff:ff:ff:ff:ff:ff') / ARP(pdst='192.168.0.1')
    # ans, unans = srp(request,timeout=2,retry=1)
    # result = []

    # for sent,received in ans:
    #     result.append({'IP':received.psrc,'MAC':received.hwsrc})
    # print(result)

    for itf in netifaces.interfaces():
        print('---',itf,'---')
        for key,val in netifaces.ifaddresses(itf).items():
            print(key,val)
        print()


if __name__ == '__main__':
    main()