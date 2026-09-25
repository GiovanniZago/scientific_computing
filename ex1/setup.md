# HomeWork 01 — Setting up a Linux machine on CloudVeneto for C programming

This guide explains how to create a Linux virtual machine on **CloudVeneto**, connect to it remotely, and install the tools required to compile and run C programs.

---

## 1. Access CloudVeneto

Open the **CloudVeneto Dashboard** and log in using the available authentication method, for example the University of Padova Single Sign-On.

After logging in, select the project associated with the Scientific Computing course.

---

## 2. Create an SSH key pair

The virtual machine can be accessed remotely through SSH using public-key authentication.

From the CloudVeneto dashboard go to:

```text
Compute → Key Pairs
```

and create a new SSH key pair.

Download the private key and save it on your local machine, for example inside:

```bash
~/.ssh/
```

Set the correct permissions with:

```bash
chmod 600 ~/.ssh/cloudveneto_key.pem
```

If an SSH key already exists on the local machine, its public key can instead be imported into CloudVeneto.

---

## 3. Create the virtual machine

From the dashboard go to:

```text
Compute → Instances → Launch Instance
```

Choose a name for the virtual machine, for example:

```text
scientific-computing
```

Select a recent **AlmaLinux** image as operating system.

Then select an appropriate **flavor**, which defines the number of CPUs, RAM and disk space available to the machine.

For the network, select the private network associated with the CloudVeneto project.

Finally, select the SSH key created in the previous step and launch the instance.

After the machine is created, take note of its private IP address.

---

## 4. Connect to the virtual machine

CloudVeneto virtual machines are normally accessed through the CloudVeneto gateway:

```text
gate.cloudveneto.it
```

For an AlmaLinux machine, the default username is:

```text
almalinux
```

The VM can be accessed with:

```bash
ssh -J <USERNAME>@gate.cloudveneto.it \
    -i ~/.ssh/cloudveneto_key.pem \
    almalinux@<VM_IP>
```

where:

- `<USERNAME>` is the username used to access CloudVeneto;
- `<VM_IP>` is the private IP address of the virtual machine.

After a successful connection, the terminal is running inside the CloudVeneto Linux machine.

---

## 5. Update the system

Before installing new software, update the installed packages:

```bash
sudo dnf update -y
```

AlmaLinux uses `dnf` as package manager.

---

## 6. Install the C development tools

Install GCC and other useful development tools with:

```bash
sudo dnf install -y gcc make gdb git
```

The main tools are:

- `gcc`: GNU C compiler;
- `make`: tool for managing compilation;
- `gdb`: debugger;
- `git`: version-control system.

Check that GCC is correctly installed:

```bash
gcc --version
```

If the command prints the GCC version, the machine is ready to compile C programs.

---

## 7. Compile and run C programs

A C source file can be compiled using:

```bash
gcc program.c -o program
```

The resulting executable can then be run with:

```bash
./program
```

For example, it is also useful to enable compiler warnings:

```bash
gcc -Wall -Wextra program.c -o program
```

If the program uses mathematical functions from `<math.h>`, the math library can be linked with:

```bash
gcc program.c -o program -lm
```

---

## Conclusion

The CloudVeneto virtual machine is now ready for C programming.

The basic workflow is:

```bash
# connect to the VM
ssh -J <USERNAME>@gate.cloudveneto.it \
    -i ~/.ssh/cloudveneto_key.pem \
    almalinux@<VM_IP>

# compile a C source file
gcc program.c -o program

# run the executable
./program
```

## References

- CloudVeneto User Guide: https://userguide.cloudveneto.it/en/latest/
- GCC documentation: https://gcc.gnu.org/