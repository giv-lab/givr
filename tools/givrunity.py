import glob
import os.path

class source:
    def __init__(self, filename):
        self.filename = filename
        self.name = os.path.basename(filename)
        self.dependencies = []
        self._parse_dependences()

    def _parse_dependences(self):
        lines = []
        with open(self.filename) as fd:
            for line in fd.readlines():
                if line.strip().startswith("#include \""):
                    dependency = os.path.basename(line.strip().replace('#include "', '').replace('"', ''))
                    if dependency != self.name:
                        self.dependencies.append(dependency)
                    continue
                if line.strip().startswith("#pragma once"):
                    continue
                lines.append(line)
        self.content = "".join(lines)

    def write_to_file(self, fd):
        fd.write(f"//------------------------------------------------------------------------------\n")
        fd.write(f"// Start {self.name}\n")
        fd.write(f"//------------------------------------------------------------------------------\n")
        fd.write(self.content)
        fd.write(f"//------------------------------------------------------------------------------\n")
        fd.write(f"// END {self.name}\n")
        fd.write(f"//------------------------------------------------------------------------------\n\n")

def unity_build():
    headers = [
        source(filename)
        for filename in glob.iglob('givr/src/**/*.h', recursive=True)
    ]
    exclude = ["givr.h"]
    headers = [h for h in headers if h.name not in exclude]
    header_dict = dict((h.name, h) for h in headers)

    with open("src/givr.h", "w") as out_fd:
        out_fd.write('#pragma once\n')
        while headers:
            written = []
            for h in headers:
                if not h.dependencies:
                    h.write_to_file(out_fd)
                    written.append(h.name)
            if not written and headers:
                print("-"*80)
                print("ERROR")
                print("-"*80)
                print([h.name for h in headers])
                raise RuntimeError("Unable to properly handle header dependencies")
            for h in headers:
                h.dependencies = list(set(h.dependencies) - set(written))
            headers = [h for h in headers if h.name not in written]

    implementations = [
        source(filename)
        for filename in glob.iglob('givr/src/**/*.cpp', recursive=True)
    ]
    exclude = [
        "stb_image.cpp",
        "tiny_obj_loader.cpp",
    ]
    implementations = [i for i in implementations if i.name not in exclude]
    with open("src/givr.cpp", "w") as out_fd:
        out_fd.write('#define TINYOBJLOADER_IMPLEMENTATION\n')
        out_fd.write('#define STB_IMAGE_IMPLEMENTATION\n')
        out_fd.write('#include "givr.h"\n\n')
        for implementation in implementations:
            implementation.write_to_file(out_fd)


if __name__ == "__main__":
    unity_build()
