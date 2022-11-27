# HITCON CTF 2022

## EaaS

* Solved: 
* Score: 

### Vulnerability
* Document.save() in the Python fitz package function has a buffer overflow [vuln](https://github.com/pymupdf/PyMuPDF/blob/a730bc6d8e77c2b39586cb826d79c7c70a1030f5/fitz/fitz.i#L1988).


### Exploit
Some protection is disabled:
* python binary's PIE
* fitz.so's canary


We can overwrite the return address because the canary is disabled. But the overwrite value has some restrictions:
1. each byte must be in the 0x00~0x7f range or the byte will be encoded.
2. cannot write a null byte.
3. a null byte will be appended to the end of the string.


So we need to do a partial overwrite on the return address. Trying to jump to an gadget near the original return address. But which gadget should we jump on?


Because the python binary is PIE disabled, it should be a great target. In my solution, I want to jump to python's interactive mode and execute arbitrary python code. After reading the python's source code, I found a point that can be used: [pymain_repl] (https://github.com/python/cpython/blob/d291a82df33cd8c917a374fef2a2373beda78b77/Modules/main.c#L543). But due to the restrictions, we couldn't write the address directly.


So we need to use the functionality that allows us to choose an additional option for Document.save(). The option will be put on the stack as an argument. So we can use the option to put the target address we want to jump to and partially overwrite the return address to a `pop pop pop... ret` gadget. Then we can bypass the restriction.


Depending on the gadget we choose, we may need to race the 1/16 or 1/4096 probability.