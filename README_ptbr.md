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

Não há palavra reservada ou simbolo especial para representar NIL em uma expressão, ele apenas é resultado de uma avaliação.

O operador de comprimento `#` resulta em `0` quando usado antes de um valor NIL.

O operador de comparação igual `==` sempre resulta em BOOLEAN Falso, e diferente `~=` sempre resulta em BOOLEAN Verdadeiro. Isso pode mudar em breve.

O Invsersor `~` transforma NIL em BOOLEAN Verdadeiro.

Qualquer outra operação com NIL resulta em NIL.

### BOOLEAN

O bom e velho Verdadeiro ou Falso.

Não há palavras reservadas ou simbolos especiais para representá-los em uma expressão, eles apenas são resultados de uma avaliação.

Valores BOOLEAN pode ser comparados entre si usando operador igual `==` e operador diferente `~=`. O valor pode ser invertido usando um inversor `~` or multiplicando por `-1`.
Qualquer outra operação resulta em NIL.

O operador de comprimento `#` resulta em `1` quando usado com BOOLEAN Verdadeiro, e resulta em `0` quando usado com BOOLEAN Falso.

### NUMBER

Valor numérico. `long double` como definido em C++11.

O valor numérico pode ser um inteiro: `25`. Ou um decimal, com a parte decimal separada por um ponto: `2.5`.
Se o número possuir apenas parte decimal é necessário colocar um zero na frente: `0.25`.
Um número inteiro também pode ser digitado em hexadecimal precedido de `0x`, as letras que fazem parte do numero pode estar maiúsculas ou minúsculas: `0x123F`.

Entre dois valores numéricos as seguintes operações podem ser realizadas:
- Adição `+`;
- Subtração `-`;
- Multiplicação `*`;
- Divisão `/`;
- Modulo (resto da divisão) `%`;
- Potência `^`;
- Comparação:
  - (maior que) `>`;
  - (menor que) `<`;
  - (maior ou igual) `>=`;
  - (menor ou igual) `<=`;
  - (igual) `==`;
  - (diferente) `~=`;

O valor numérico pode ser convertido para inteiro (i.e. descartar a parte decimal) usando o operador de comprimento `#` antes do valor.

Pode ser convertido para BOOLEAN Falso com um Inversor `~` antes do valor.

Pode ser avaliado como não-zero com Executar `!` antes do valor, isso resulta em BOOLEAN Falso com zero e BOOLEAN Verdadeiro com outros valores.

### STRING

Cadeia de caracteres. Definida por caracteres cercados por um par de aspas duplas `"` or um par de aspas simples `'`. Note que se precisar de aspas simples dentro da string, pode se usar aspas duplas para delimitar a string, e vice-versa. Também note que uma string pode ter várias linhas.

As seguintes operações podem ser realizadas em valores de STRING:
- Concatenação `+`
  - Concatena a STRING do lado esquerdo (lhs) com a representação STRING do valor do lado direito (rhs).
- Subtração `-`
  - Remove a primeira ocorrência de rhs de lhs.
- Multiplicação `*`
  - Quando usado com um valor numérico, repete a string essa quantidade de vezes;
  - Quando usado com uma referência a nó, executa o nó para cada caracter da string, passando os caracteres como argumento um por vez, resulta em uma lista com todos os resultados da execução, exceto quando esse resultado é NIL.
- Divisão `/`
  - Quando usado com um valor numérico, corta a string e resulta em uma string com no maximo esse numero de caracteres. Se o número > 0 pega os primeiros caracteres, se o número < 0 pega os últimos caracteres;
  - Quando usado com outra string, divide lhs usando rhs como divisor. Resulta em uma lista com os pedaços da string original;
  - Quando usado com uma referência a nó, divide a string usando a execução do nó como divisor. Resulta em uma lista com os pedaços da string original.
- Encontrar `%`
  - Quando usado com valor numérico, resulta no caracter na posição do número passado, ou NIL se a posição estiver fora da string;
  - Quando usado com outra string, encontra a posição de lhs dentro de rhs, ou NIL se rhs não for uma substring de lhs.
- Comparação `>`, `<`, `>=`, `<=`, `==`, `~=`
  - Compara lexicograficamente as duas string passadas.

O operador de comprimento `#` pode ser usado para resultar na quantidade de caracteres da string.

O Inversor `~` converte uma string em BOOLEAN Falso.

O Executar `!` pode ser usado para testar se a string é vazia, resulta em BOOLEAN Falso com string vazia, BOOLEAN Verdadeiro com string não vazia.

### LIST

Uma lista é uma coleção de valores armazenados de forma consecutiva, como um array na maioria das linguagens.

É definida por uma lista de expressões cercada por um abre e um fecha chaves `{}`.

As seguintes operações podem ser realizadas em listas:

- Acrescentar `+`
  - Coloca um valor de qualquer tipo no fim da lista.
- Remover `-`
  - Usado com um valor numérico, remove o item da posição passada.
- Multiplicação `*`
  - Quando usado com outra lista, concatena as duas listas;
  - Quando usado com um valor numérico, repete a lista a quantidade de vezes passada se o valor >= 0;
  - Quando usado com um nó, executa o nó para cada item da lista, passando os itens como argumento um por vez, resulta em uma lista com os resultados exceto quando o resultado for NIL.
- Divisão `/`
  - Quando usado com um valor numérico, corta a lista e resulta em uma lista com, no máximo, essa quantidade de itens. Se o número > 0 pega os primeiros itens, se número < 0 pega os últimos itens;
  - Quando usado com um nó funciona como filtro, resulta em uma lista com todos os itens da lista original que, quando avaliado pelo nó, resultaram em _verdadeiril_.
- Indice `%`
  - Usado com um valor numérico, resulta no item da lista que estiver na posição passada, ou NIL se a posição estiver fora da lista.
- Encontrar `<`
  - Usado com um nó, resulta no indice do primeiro item da lista que, quando avaliado pelo nó, resulta em _verdadeiril_. Ou NIL se nenhum item resultar _verdadeiril_.
- Encontrar igual `<=`
  - Encontra o indice da primeira ocorrência de rhs na lista, ou NIL se rhs não estiver na lista.
- Reduzir `>`
  - Usado com um nó;
  - Se a lista estiver vazia, resulta em NIL;
  - Se a lista só possuir um item, resulta neste item;
  - Nos outros casos, é feita uma varredura na lista aplicando o nó com o resultado da última iteração e o item atual como argumentos. Resulta no resultado final da varredura.
- Comparação `==`, `~=`
  - Usado com outra lista, compara cada valor das duas listas de acordo com o indice:
    - para `==`, verdadeiro apenas se todos os itens forem iguais nos dois;
    - para `~=`, verdadeiro se qualquer item de mesmo indice for diferente entre as duas listas.

O operador de comprimento `#` pode ser usado para avaliar a quantidade de itens na lista.

O Inversor `~` resulta em BOOLEAN Falso.

O Executar `!` pode ser usado para testar se a lista possui itens, resulta em BOOLEAN Falso com lista vazia, BOOLEAN Verdadeiro com lista não vazia.

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
