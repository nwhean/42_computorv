```mermaid
classDiagram
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
		+Type type
		+gen() Expr
		+reduce() Expr
		+to_string() String
		+eval() Token
		+get_op() Token
		+set_type(Type)
		+get_type() Type
	}

	Object <|-- Node
	Expr <|-- Op
	Expr <|-- Unary

	class Unary {
		Expr expr;
	}


	%% lexer

	Object <|-- Lexer

	class Lexer {
		-char peek
		+scan() Token
	}

	Numeric <|-- Num

	class Num {
		+int value
	}

	Token <|-- Numeric

	class Numeric {
		<<abstract>>
		+add(Numeric) Numeric*
		+sub(Numeric) Numeric*
		+mul(Numeric) Numeric*
		+div(Numeric) Numeric*
		+unary() Numeric*
	}

	Numeric <|-- Real

	class Real {
		+double value
	}

	Object <|-- Token

	class Token {
		+int tag
		+copy() Token
		+to_string() String
		#get_tag() int
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


	%% symbols

	Word <|-- Type

	class Type {
		+int width
	}
```