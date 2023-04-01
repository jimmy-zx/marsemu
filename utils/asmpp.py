"""Assembly preprocessor

Features:
- Expand `.include`
- Expand `.macro` (default values ommited)
- Evaluates constatns like `1 + 1($s0)`
"""

import sys
import re

def process_include(src: list[str]) -> list[str]:
    """Embed every .include instruction"""
    res: list[str] = []
    for n, line in enumerate(src):
        line = line.strip("\n")
        if ".include" in line:
            cmd, file = line.strip().split()
            if cmd != ".include":
                raise Exception(f"{n}: invalid .include statement")
            res.append(f"# {line}")
            with open(file.strip('"'), "r") as f:
                res.extend([line.strip("\n") for line in f])
            continue
        res.append(line)
    return res


class Macro:
    def __init__(self, name: str, header: str, content: str) -> None:
        self.name = name
        self.content = content
        self.params: list[str] = []
        for param in header.split()[2:]:
            if "=" not in param:
                self.params.append(param)
                continue
            self.params.append(param.split("=")[0])

    def apply(self, line: str) -> list[str]:
        assert self.name == line.split()[0]
        args = line.split()[1:]
        assert len(args) == len(self.params)
        res: list[str] = []
        for line in self.content:
            for i, param in enumerate(self.params):
                line = line.replace(f"\\{param}", args[i])
            res.append(line)
        return res

    def __str__(self) -> str:
        return "{}({})".format(self.name, ", ".join(self.params))

    def __repr__(self) -> str:
        return str(self)


def get_macros(src: list[str]) -> tuple[dict[str, Macro], list[str]]:
    """Get a dict of macros, and strip off lines related to macros"""
    macroname: str = None
    macrohdr: str = None
    macrobody: list[str] = None
    macros: dict[str, tuple[str, list[str]]] = {}
    res: list[str] = []
    for n, line in enumerate(src):
        if ".macro" in line:
            if macrohdr is not None:
                raise Exception(f"{n}: nested macro not allowed")
            macrohdr = line
            macroname = line.split()[1]
            macrobody = []
            continue
        if ".endm" in line:
            if macrohdr is None:
                raise Exception(f"{n}: invalid .endm")
            macros[macroname] = Macro(macroname, macrohdr, macrobody)
            macrohdr = None
            continue
        if macrohdr is not None:
            macrobody.append(line)
            continue
        res.append(line)

    return macros, res


def process_macro(src: list[str], macros: dict[str, tuple[Macro]]) -> list[str]:
    """Expand macros"""
    res: list[str] = []
    for n, line in enumerate(src):
        if not len(line.strip()):
            res.append(line)
            continue
        opname = line.split()[0]
        if opname in macros:
            res.append(f"# {line}")
            res.extend(macros[opname].apply(line))
            continue
        res.append(line)
    return res


def process_constant(src: list[str]) -> list[str]:
    res: list[str] = []
    for n, line in enumerate(src):
        if not line.strip() or "." in line or ":" in line:
            res.append(line)
            continue
        comment = "#".join(line.split("#")[1:])
        if comment:
            comment = "#" + comment
        instr = line.split("#")[0]
        if not instr.strip():
            res.append(line)
            continue
        opcode, args = instr.split()
        comps = args.split(",")
        for i, comp in enumerate(comps):
            match = re.match(r"(.*)\((.*)\)", comp)
            if not match:
                continue
            assert len(match.groups()) == 2
            comps[i] = "{}({})".format(eval(match.groups()[0]), match.groups()[1])
        res.append("\t{}\t{}\t{}".format(opcode, ",".join(comps), comment))
    return res


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        src = f.readlines()
    p1 = process_include(src)
    macros, p2 = get_macros(p1)
    p3 = process_macro(p2, macros)
    p4 = process_constant(p3)
    print("\n".join(p4))
