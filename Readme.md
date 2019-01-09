this project attempts to make a succesfull disassembler generator

the generator shall be written once the parser is complete, as you cannot generate an incomplete and faulty parser

the generator shall generate the parser using an AST provided via https://github.com/orangeduck/mpc 's parser combinator library, this and the generator that uses it, is not included in this repo as it is not needed at this time

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



here is an example of this:

```
	struct TrieNode *root = getNode(); 
	struct TrieNode *sub_root = getNode(); 
	struct TrieNode *rule_1 = getNode();
	struct TrieNode *rule_2 = getNode(); 
	struct TrieNode *rule_3 = getNode(); 
	struct TrieNode *rule_4 = getNode(); 
	struct TrieNode *rule_5 = getNode(); 
	struct TrieNode *rule_6 = getNode(); 
	struct TrieNode *rule_7 = getNode(); 
	struct TrieNode *rule_8 = getNode(); 

	insert(root, "12356", 0, NULL);
	insert(sub_root, "1", 0, rule_1);
		insert(rule_1, "2", 1, rule_2);
		insert(rule_2, "3", 2, rule_3);
		insert(rule_3, "5", 3, rule_4);
		insert(rule_4, "6", 4, NULL);
	
	insert(root, "12456", 0, NULL);
	insert(sub_root, "1", 0, rule_5);
		insert(rule_5, "2", 1, rule_6);
		insert(rule_6, "4", 2, rule_7);
		insert(rule_7, "5", 3, rule_8);
		insert(rule_8, "6", 4, NULL);
		
	// Search for different keys
	isp(root, sub_root, "12356");
	isp(root, sub_root, "12456");
```

in this example, the disired execution is as follows

parse `12356`
match `1`
choose chain 0
match `2`
match `3`
match `5`
match `6`

parse `12456`
match `1`
match `2`
fail `3`
backtrack to `1`
choose chain 1
match `2`
match `4`
match `5`
match `6`

the following is how the parser currently works:

the parser creates a `root` trie, this contains all the possible combinations of parser results, this is required

the parser then creates a network of segmented trie's, this is required for the implementation of conditional code since partial trie matching cannot be done via the root


in this trie network:

the parser then creates a `sub_root` trie, this contains the first intended match of every rule

the parser then creates sub trie's and binds them accordingly to form a trie chain, this is needed to correctly detect trie sequences and isolate trie's from detecting other matches in trie's not related to the trie chain

backtracking is required in the trie chain

a reference shall be interperated via a trie chain, it is uncertain how a `multi-match` rule will be interperated yet

in wich a `multi-match` rule is a rule wich has multiple possibilities but the rule only terminates when the final match is made, allowing for multiple simutanious matches in a single rule

for example:

```
CN:
  // [] represent a muli-match parse, in where the same string can be matched multiple times without terminating to next rule after a single match
    
    0xx1: none //jump if test signal of 4004 is = 0
   [1xx1]: none //jump if test signal of 4004 is = 1
   [0x1x]: none //jump if carry bit = 1
   [1x1x]: none //jump if carry bit = 0
   [01xx]: none //jump if accumulator = 0
   [11xx]: none //jump if accumulator != 0

```

in this, the rule `CN` will only complete once `11xx` has been attempted to be matched
