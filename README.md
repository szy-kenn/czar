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
./run [cz-file] [output-txt-file]
```

In Linux:

```bash
./run.sh [cz-file] [output-txt-file]
```

## 🟢 Examples

Write a czar code in a `.cz` file, for example:

```czar
fixed my_str: str = "Hello, World!"
```

Run the program:

```bash
./run test.cz output.txt
```

And the output file *(output.txt)* should look like this:

> [!CAUTION]
> The lexical analyzer isn't finished yet so there might be some issues in the output

```output.txt
TOKEN TYPE     VALUE
====================
T_FIXED        fixed
T_ID           my_str
T_COLON        :
T_DTYPE        str
T_EQL          =
T_DQUOTE       "
T_STR          Hello, World!
T_DQUOTE       "
T_EOF          EOF
```
