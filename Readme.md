this project atttempts to make a succesfull disassembler generator

the generator shall be written once the parser is complete, as you cannot generate an incomplete and faulty parser

the parser is located in https://github.com/mgood7123/disassemblergen/blob/master/vcpu/trie_binary.c (source file) and https://github.com/mgood7123/disassemblergen/blob/master/vcpu/trie_binary (executable ELF binary, 64 bit)

the parser must be able to support the implementation of parser rules

an example of this is the following:

```
RP:
    000
    001
    010
    011
    100
    101
    110
    111
FIN1: |0011|RP|0|
FIN2: |0011|RP|1|
```

where `FIN1` is the an `instruction` RULE, or `root` in which the parser will start at, and progress downwards through the roots

`RP` is a `definition` RULE, or a `reference to a definition`

the following behaviour is expected, given `00110101`


parser starts at `FIN1`

attempts to match `0011` and suceeds

sees a reference to `RP`

dereferences it then attempts to find a match to any of its rules

attempts to match `000` and fails

attempts to match `001` and fails

attempts to match `010` and suceeds

attempts to match `0` and fails, proceeds to `FIN2`

parser starts at `FIN2`

attempts to match `0011` and suceeds

sees a reference to `RP`

dereferences it then attempts to find a match to any of its rules

attempts to match `000` and fails

attempts to match `001` and fails

attempts to match `010` and suceeds

attempts to match `1` and suceeds

`FIN2` is matched

parser ends execution
