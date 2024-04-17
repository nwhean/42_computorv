```mermaid
classDiagram
	%% container
	class Container {
		empty() bool
		size() size_t
		clear() void
		insert(void *) bool
		erase(void *) size_t
		find(void *)
		reserve(size_t) void
	}

	Container <|-- UnorderedMap

	class UnorderedMap {
		-size_t size
		-size_t capacity
		-void **key
		-void **value
		-func_ptr key_equal
	}


	%% inter

	Op <|-- Arith

	class Arith {
		+Expr expr1
		+Expr expr2
	}

	Expr <|-- Constant
	Node <|-- Expr

	class Expr {
		+Token op
		+Tag tag
		+gen() Expr
		+reduce() Expr
		+to_string() String
		+eval() Token
		+get_op() Token
		+set_tag(Tag)
		+get_tag() Tag
	}

	Object <|-- Node
	Expr <|-- Op
	Op <|-- Unary

	class Unary {
		Expr expr;
	}


	%% lexer

	Object <|-- Lexer

	class Lexer {
		-char peek
		+scan() Token
	}

	Token <|-- Numeric

	class Numeric {
		<<abstract>>
		+add(Numeric) Numeric*
		+sub(Numeric) Numeric*
		+mul(Numeric) Numeric*
		+div(Numeric) Numeric*
		+mod(Numeric) Numeric*
		+pos() Numeric*
		+neg() Numeric*
	}

	Numeric <|-- Rational

	class Rational {
		+long numerator
		+long denominator
	}

	Object <|-- Token

	class Tag {
		<<enumeration>>
		PLUS
		MINUS
		RATIONAL
		WORD
		ID
	}

	class Token {
		+Tag tag
		+copy() Token
		+to_string() String
		+get_tag() Tag
	}

	Token <|-- Word

	class Word {
		+String lexeme
	}


	%% parser

	Object <|-- Parser

	class Parser {
		-Lexer lexer
		-Token look
		+program()
	}
```
