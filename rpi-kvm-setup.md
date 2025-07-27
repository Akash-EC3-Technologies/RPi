# KVM/QEMU, libvirt, and virt-manager Setup on Raspberry Pi 5

## 2. Install Required Packages

```bash
sudo apt install -y qemu-kvm libvirt-daemon-system libvirt-clients bridge-utils virtinst virt-manager
```

## 3. Add Current User to libvirt and kvm Groups

```bash
sudo usermod -aG libvirt $(whoami)
sudo usermod -aG kvm $(whoami)
```

> **Note**: Log out and log back in or reboot for group changes to take effect.

## 4. Enable and Start libvirtd Service

```bash
sudo systemctl enable libvirtd
sudo systemctl start libvirtd
```

## 5. Validate Installation

```bash
virt-host-validate
```

## Done

You can now manage virtual machines using `virt-manager` GUI or command-line tools.
