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
fixed name: str
name = "Czar"
```

Run the program:

```bash
./run test.cz output.txt
```

The output file for the symbol table should look like this:

```output.txt
T_FIXED        fixed
T_IDENT        name
T_COLON        :
T_DT_STR       str
T_IDENT        name
T_EQL          =
T_STR          "Czar"
T_EOF          EOF
```

The Abstract Syntax Tree should look like this:

```ast.txt
P_program:
    P_assign_stmt:
        Identifier Name: name
        Value:
    P_decl_stmt:
        Mutability Type: fixed
        Identifier Name: name
        Data type: str
```
