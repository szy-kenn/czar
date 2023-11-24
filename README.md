<h1 align='center'>czar</h1>

## 📝 Prerequisites

> [!IMPORTANT]
> Before starting, make sure to have the following tools installed in your computer:
> - GCC (GNU Compiler Collection)

You can run the following command to check if gcc is already installed:

```bash
gcc --version
```

## ⚙️ How To Run

To start, run the following command:

In Windows:

```bash
./run [cz-file]
```

In Linux:

```bash
./run.sh [cz-file]
```

## 🟢 Examples

Write a czar code in a `.cz` file, for example:

```czar
my_str: string = "Hello, World!"
```

Run the program:

```bash
./run test.cz
```

And the output file should look like this:

```plaintext
TOKEN TYPE     VALUE
====================
T_ID           my_str
T_DTYPE        str
T_EQL          =
T_DQUOTE       "
T_STR          Hello, World!
T_DQUOTE       "
```
