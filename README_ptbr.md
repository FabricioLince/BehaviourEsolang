# BehaviourEsolang
Minha Esolang chamada Behaviour

Disponível para experimentar no [Replit](https://replit.com/@fabriciorodrigues/bhv)

## Sumário
- [Introdução](#the-basics)
- [Tipos](#types)
  - [NIL](#nil)
  - [BOOLEAN](#boolean)
  - [NUMBER](#number)
  - [STRING](#string)
  - [LIST](#list)
  - [NODE](#node)
- [Nós de controle](#control-nodes)
  - [Sequenciador](#sequencer)
  - [Selecionador](#selector)
  - [Repetidor](#repeater)
  - [Operador Se](#if-operator)
  - [Opcional](#optional)
  - [Negador](#negator)
- [Referenciamento de Nós](#node-referencing)
- [Algoritmos de Exemplo](#example-algorithms)
  - [FizzBuzz](#fizzbuzz)
  - [Fatorial](#factorial)


## Introdução

A sintaxe de Behaviour é inspirada na [Árvore Comportamental](https://en.wikipedia.org/wiki/Behavior_tree_(artificial_intelligence,_robotics_and_control))

Todo comando em Behaviour é uma _Expressão_.

Toda _Expressão_ é convertida em um _Nó_ quando analisada.

Todo _Nó_ pode ser avaliado para retirar seu valor.

Um script em Behaviour se torna uma árvore de expressões cuja execução imita uma árvore comportamental.

O script a seguir é um exemplo de uma Expressão de Atribuição, onde a Variável `count` recebe o valor `12`:

`count = 12`

Para imprimir um valor no console podemos usar uma Expressão de Impressão, colocando um arroba `@` na frente da Expressão cujo valor deverá ser impresso:

`@"Olá mundo!"` vai imprimir a string `Olá mundo!` e ir para a próxima linha no console.

Colocar ponto-e-virgula `;` no fim das Expressões não é necessário, mas pode ser feito para organizar o código.


## Tipos

Os tipos básicos para valores em Behaviour são:
- NIL
  - Representa um erro ou um valor vazio
- BOOLEAN
  - Verdadeiro ou Falso
- NUMBER
  - Número decimal de ponto flutuante
- STRING
  - Cadeia de caracteres
- LIST
  - Lista de valores de qualquer tipo
- NODE
  - Uma referência a um Nó

Um valor é _verdadeiril_ quando é qualquer valor exceto NIL ou BOOLEAN Falso.

Um valor é _falsil_ quando é NIL ou BOOLEAN Falso.

### NIL

NIL pode representar um valor vazio, como uma Variável não inicializada, ou uma posição fora de alcance em uma STRING ou LIST.
Também são usados para representar uma situação de erro, como uma operação inválida.

There is no reserved word or symbol to represent it in an expression, it can only be achieved as a result for an evaluation.

The Length operator `#` evaluates to `0` when used before a NIL value.

The Comparison operators equals `==` always evaluates to BOOLEAN False, and not equals `~=` always evaluates to BOOLEAN True. This may change in the near future.

Any other operation with NIL will be evaluated to NIL. 

### BOOLEAN

The regular old True or False.

There is no reserved word or symbol to represent them in an expression, they can only be achieved as a result for an evaluation.

BOOLEAN values can be compared against each other using `==` and `~=`. The value can be inverted using `~` or by multiplying it by `-1`. 
Any other operation will result in NIL.

The Length operator `#` evaluates to `1` when used with BOOLEAN True, and evaluates to `0` when used with BOOLEAN False.

### NUMBER

Numerical value. Long double value as defined in C++.

You can type a integer value like this: `25`. Or a decimal value with the decimal portion separated by a dot like this: `2.5`. 
If the number only has a decimal part you need to preceed with zero, like this: `0.25`.

Between two NUMBER values the following operations can be performed:
- Addition `+`;
- Subtraction `-`;
- Multiplication `*`;
- Division `/`;
- Modulo (division remainder) `%`;
- Power `^`;
- Comparison:
  - (bigger than) `>`;
  - (less than) `<`;
  - (bigger equal than) `>=`;
  - (less equal than) `<=`;
  - (equals) `==`;
  - (not equals) `~=`;

The NUMBER can be converted to integer (i.e. discard the decimal part) using Length operator `#` before it.

It can be evaluated to BOOLEAN False with Negator `~` before it.

It can be evaluated as non-zero with Execute `!` before it, this evaluates to BOOLEAN False when zero, BOOLEAN True otherwise.

### STRING

String of characters. Defined by characters surrounded by double quotes `"` or surrounded by single quotes `'`. Notice that if you want single quotes inside your string you may use double quotes to delimited it, and vice versa. Also notice that a string can span multiple lines.

The following operations can be performed on STRING values:
- Concatenation `+`
  - Concatenate the lhs STRING with the string representation of the rhs value.
- Subtraction `-`
  - Remove the first occurence of rhs from lhs
- Multiplication `*`
  - When used with a NUMBER value, repeat the STRING value that number of times;
  - When used with a NODE value, executes the NODE for each character of the STRING, passing said character as argument each time, evaluates to a LIST with all the results except when said result is NIL (so it can be used as a filter).
- Division `/`
  - When used with a NUMBER value, clips the string to a string with atmost the number of characters, if rhs > 0 gets the first rhs characters, if rhs < 0 gets the last rhs characters;
  - When used with a STRING value, splits lhs by rhs, evaluates to a LIST with the resulting items;
  - When used with a NODE value, splits the string using the node as divisor, evaluates to a LIST with resulting items.
- Finding `%`
  - When used with a NUMBER value, gets the character in the position from the string, NIL if the position is out of range;
  - When used with a STRING value, finds the rhs position within lhs, NIL if rhs isn't a substring of lhs.
- Comparison `>`, `<`, `>=`, `<=`, `==`, `~=`
  - Compares lexicographically the two STRING values.

The Length operator `#` can be used to evaluate the amount of characters in the STRING.

It can be evaluated to BOOLEAN False with Negator `~` before it.

The Execute `!` can be use to test a non-empty STRING, evaluating to BOOLEAN False on a empty STRING, BOOLEAN True otherwise.

### LIST

A LIST is a collection of values stored consecutively, like an array on most languages. 

It is defined by a list of Expressions surrounded by curly brackets `{}`

The following operations can be performed on LIST values:

- Append `+`
  - Put a value of any type in the end of the LIST.
- Remove `-`
  - Used with a NUMBER value, remove the item in the position passed.
- Multiplication `*`
  - When used with another LIST, concatenate the two LIST values;
  - When used with a NUMBER value, repeat the list the amount passed if the amount is >= 0;
  - When used with a NODE value, executes the NODE for each item of the LIST, passing said item as argument each time, evaluates to a LIST with all the results except when said result is NIL (so it can be used as a filter).
- Division `/`
  - Used with a NUMBER value, clips the LIST to a list with atmost NUMBER items, if number > 0 gets the first number items, if number < 0 gets the last number items;
  - Used with a NODE value, works as a filter, evaluates to a LIST with all the members of the original LIST that evaluated truthy when passed to the NODE.
- Index `%`
  - used with a NUMBER value, gets the item in the number position from the list, NIL if the position is out of range.
- Find `<`
  - used with a NODE value, find the first index of the list that evaluates truthy when passed to the NODE. NIL if no item evaluated truthy.
- Find Equal `<=`
  - find the index of the first occurence of rhs in the LIST. NIL if it couldn't find the item.
- Reduce `>`
  - Used with a NODE value;
  - if the LIST is empty evaluates to NIL;
  - if lhs only has one item, evaluates to the first item;
  - otherwise loops through the list applying the NODE with the result of the last iteration and the current item as arguments
- Comparison `==`, `~=`
  - Used with another LIST, compares each value of lhs with the corresponding index of rhs:
    - for `==`, true only if every index are equals on both;
    - for `~=`, true if any index is differente on both.

The Length operator `#` can be used to evaluate the amount of items in the LIST.

It can be evaluated to BOOLEAN False with Negator `~` before it.

The Execute `!` can be use to test a non-empty LIST, evaluating to BOOLEAN False on a empty LIST, BOOLEAN True otherwise.

### NODE

A NODE is a reference to a parsed Expression that can be evaluated later. More at [Node Referencing](#node-referencing).

This works essencially as a first class function, since it can be stored in Variable and used in Expression like any other value.

A function defined in C++ Engine, and injected in the Datatable works pretty much exactly like a NODE value, 
the only difference being if you inspect its type or print it, you're gonna see "CFUNC" instead of "NODE".

Other than the operators already specified that accepts NODE values, the following operators are valid:

- Comparison `==` and `~=`:
  - Checks if the two NODES are the same or not.

The Length operator `#` always evaluates to `0` when used before a NODE value. 

The Negator `~` evaluates to a inverted NODE value. This value can be used and executed like a regular NODE value, 
the difference being that it evaluates to the negated version of the original NODE result.

## Control Nodes

There are Sequencers, Selectors, Repeaters and the If Operator, these are the control Nodes.

We also have the Optional and the Negator, which modify the resulting evaluation of a Node.


### Sequencer

The Sequencer is denoted by a list of zero or more Expressions inside parentheses `()`.

When evaluated the Sequencer will evaluate each of its _child Nodes_ in order, 
this evaluation will stop if:
- A child Node is evaluated falsy,
- Or all children were evaluated truthy.

The Sequencer Node assumes the value of its last evaluated child Node. 

In another words, the Sequencer will be succesfull if all of its children are evaluated successfully, in order.

For example, the following Sequencer will evaluate each addition and finally assume the value of the final addition:

`seq = (2+3; 5+9; 6+13)`

Since none of those addition will cause any problems, by the end, the value of `seq` will be `19`, the value of the last addition in the Sequencer.

An empty Sequencer will be evaluated to NIL.

### Selector

The Selector is denoted by a list of zero or more Expressions inside square brackets `[]`.

When evaluated the Selector will evaluate each of its _child Nodes_ in order,
this evaluation will stop on the first _child Node_ that is evaluated truthy.
The Selector assumes the value of the first child that evaluated truthy, or NIL if none of them did.

In another words, the Selector "selects" the value of the first child that evaluated succesfully.

For example, the following Selector will evaluate its children and stop when the second one is evaluated:

`sel = [name; 2+2; 10*3]`

Since `name` is not defined its value is NIL, then the selector evaluates the addition `2+2` and stops. 
By the end, the value of `sel` will be `4`.
The multiplication `10*3` is never evaluated.

An empty Selector will be evaluated to NIL.

### Repeater

The Repeater is denoted by the back-slash `\` followed by a single Expression.

When evaluated the Repeater will evaluate its child node repeatedly until the child evaluates truthy.
When this happens the Repeater stops and assumes the value BOOLEAN True.

For example, the following script will initialize a Variable `i` with the value `0`, then repeat a Sequencer that increments `i` by `1` and tests if `i` is bigger than `9`.
It will stop when `i` equals `10`:

```
i=0
\(i+=1; i>9)
```

Note that to repeat both the increment and the comparison each time I have to put them inside a Sequencer.

### If Operator

The pipe symbol `|` between two Expressions is called the If Operator, and its used to evaluate to the left hand side Expression value if the test on the right evaluates truthy, the If Operator evaluates to NIL otherwise. 

There are two main cases:

- With a NODE on the right hand side
  - In this case the NODE is evaluated using the left hand side value as argument.
- With anything else on the right hand side
  - The truthy value of the rhs is used as a condition.

It's often used as an alternative to the Selector, on in conjunction to the Selector Node.

For example, the following pairs of lines are equivalent:
```
[(func:a; a)]
a|func

[(func:a; a) b]
[a|func b]
```

### Optional

The Optional is denoted by the interrogation mark `?` followed by a single Expression.

The Optional is used to ignore a falsy evaluation of the child Node. Making it always evaluate to BOOLEAN True.

For example, if you have a Sequencer with an Expression that may evaluate falsy, but you don't want the Sequencer to stop because of it, 
you put child Expression inside a Optional:

`(?mayEvaluateFalsy; needsToBeEvaluatedRegardless)`

In this script, all the two children Expressions will be evaluated.

### Negator

The Negator is denoted by the tilde `~` followed by a single Expression.

The Negator is used to convert truthy values into BOOLEAN False, and falsy values into BOOLEAN True.

In another words, if the child Node evaluates to NIL or to BOOLEAN False, the Negator evaluates to BOOLEAN True. 
Otherwise it evaluates to BOOLEAN False.

For example, the following script has a Negator followed by a Expression that evaluates to BOOLEAN False. 
The Negator then evaluates to BOOLEAN True:

`~(1>2)`

In this case we have to use the parentheses to make sure the Negator is negating the evaluation of the whole comparison, 
and not the value of the number `1` (which would result in BOOLEAN False, and when compared to `2` would result in NIL)


## Node Referencing

in Behaviour, Node Referencing is the equivalent of functions in most languages.

With Node Referencing you can put a Node inside a Variable to be evaluated later.

To extract the Node from an Expression use the ampersand `&` followed by a single Expression.

For example, the following script will create a reference for a Node that adds the values of `a` and `b`:

`sum = &a+b`

Now the Variable `sum` has a reference to a NODE that adds `a` and `b`. 
To then evaluate the referenced node use the exclamation mark `!` followed by the variable that contains the Node:

`result = !sum`

Note that the Referenced Node expects a value for the Variable `a` and `b`, so we need to define them before evaluating the Node:

```
a = 2
b = 3
result = !sum
```

For convenience you can assign the values like this:

`result = !sum:a=2,b=3`

Or like this:

`result = !sum:2,3`

The last example works because when passing values to a Node evaluation, if you don't specify which variable you want to put them on, 
it's assumed to be the Variables `a`, `b`, `c`, `d` and `e`, in that order.

You can also ommit the exclamation point if you are passing values, like this:

`result = sum:2,3`


## Example Algorithms

### FizzBuzz

The fizz-buzz algorithm is good to demonstrate simple looping and conditionals,
in this example I will print the fizz buzz results from 1 to 50:

```
i = 1
\(
  ?mod3 = i%3 == 0
  ?mod5 = i%5 == 0
  [
    (mod3; mod5; @"fizzbuzz")
    (mod3; @"fizz")
    (mod5; @"buzz")
    @i
  ]
  i += 1
  i > 50
)
```

Explanation:

First initialize `i` to `1` and enter a Repeater Sequencer. Inside the Sequencer, assigns `mod3` with the info for if the Variable `i` is divisible by `3`, 
and `mod5` if the Variable `i` is divisible by `5`. Since said assigns can result in BOOLEAN False, they can stop the Sequencer, so we preceed the assign with `?` 
to make the Sequencer ignore the assign evaluation.

Then a Selector tests each of the possibilities and prints accordingly.

Finnaly we increment `i` by `1` and test if `i` is bigger than `50`. When this final test evaluates BOOLEAN True, the Sequencer ends in BOOLEAN True, 
allowing the Repeater to stop repeating.

### Factorial

The factorial algorithm is a good example for demonstrating recursion:

```
factorial = &[
  (a < 2; 1)
  a * factorial:a-1
]
```

Explanation: 

The main Selector will evaluate if the value passed `a` is less than `2`, if it is then it's evaluated to `1` and it's done.

Otherwise, it multiplies the value of `a` to the result of `factorial` when called with `a-1`.