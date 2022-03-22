# BehaviourEsolang
Minha Esolang chamada Behaviour

Disponível para experimentar no [Replit](https://replit.com/@fabriciorodrigues/bhv)

## Sumário
- [Introdução](#introdução)
- [Tipos](#tipos)
  - [NIL](#nil)
  - [BOOLEAN](#boolean)
  - [NUMBER](#number)
  - [STRING](#string)
  - [LIST](#list)
  - [NODE](#node)
- [Nós de controle](#nós-de-controle)
  - [Sequenciador](#sequenciador)
  - [Selecionador](#seletor)
  - [Repetidor](#repetidor)
  - [Operador Se](#operador-se)
  - [Opcional](#opcional)
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

O Executar `!` pode ser usado para testar se a string tem conteúdo, resulta em BOOLEAN Falso com string vazia, BOOLEAN Verdadeiro com string não vazia.

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

Um NODE (nó) é uma referência a uma expressão que foi analisada e pode ser avaliada em outro momento. Mais em [Referenciando Nós](#node-referencing).

Funciona basicamente como uma função de primeira classe, já que pode ser guardada em variáveis e usada em expressões como qualquer outro valor.

Uma função definida na Engine C++, e injetada na Datatable funciona exatamente como um valor de nó.
A única diferença será quando o valor for inspecionado ou impresso, onde aparecerá "CFUNC" ao invés de "NODE".

Além dos operadores já especificados que aceitam valores de nó, as seguintes operações são validas:

- Comparação `==` e `~=`:
  - Checa se os dois nós passados são o mesmo, i.e. apontam para o mesmo lugar na memória.

O operador de comprimento sempre resulta em `0` quando usado com valor de nó. Isso pode mudar em breve.

O Inversor `~` resulta em um nó cujo resultado será invertido em relação ao original. 

## Nós de Controle

Nós de controle são nós que ditam como suas sub expressões são avaliadas.

Existem Sequenciadores, Seletores, Repetidores e o Operador Se.

Também há o Opcional e o Inversor, que modificam o resultado de avaliação da sub expressão.

### Sequenciador

O Sequenciador é denotado por uma lista de zero ou mais expressões entre parênteses `()`.

Quando avaliado, o Sequenciador irá avaliar cada uma das sub expressões na ordem que estiverem.
Esta avaliação irá parar se:
- Uma sub expressão tem como resultado um valor _falsil_;
- Ou todas as sub expressões resultaram em valor _verdadeiril_.

O Sequenciador assume o valor da última sub expressão que foi avaliada.

Em outras palavras, o Sequenciador terá sucesso se todos as sub expressões tiverem sucesso, em ordem.

Por exemplo, o Sequenciador a seguir irá avaliar cada adição e por fim asumirá o valor da adição final:

`seq = (2+3; 5+9; 6+13)`

Considerando que nenhuma dessas adições causará problemas, no fim da execução o valor de `seq` será `19`, que é o valor da última sub expressão `6+13;`.

Um Sequenciador vazio terá como resultado o valor NIL.

### Seletor

Um Seletor é denotado por uma lista de zero ou mais expressões entre colchetes `[]`.

Quando avaliado, o Seletor irá avaliar cada uma das sub expressões em ordem,
essa avalição irá parar na primeira sub expressão que tiver como resultado um valor _verdadeiril_.
O Seletor então assume o valor da primeira sub expressão que resultou em _verdadeiril_, 
ou NIL se todas as sub expressões resultaram em _falsil_.

Em outras palavras, o Seletor "seleciona" o valor da primeira expressão que tiver sucesso na avaliação.

Por exemplo, o Seletor a seguir irá avaliar as sub expressões e parar quando a segunda for avaliada:

`sel = [nome; 2+2; 10*3]`

Já que `nome` não foi definido seu valor é NIL, então o Seletor avalia a adição `2+2;` e para.
No fim da execução o valor de `sel` será  `4`.
A multiplicação `10*3;` nunca será avaliada.

Um Seletor vazio terá como resultado o valor NIL.

### Repetidor

Um Repetidor é denotado por uma barra invertida `\` seguida de uma única sub expressão.
Ou uma barra invertida `\`, seguida de uma variável ou número, seguida de outra barra invertida e por fim uma única sub expressão. 
Neste caso a variável ou número é o limite do repetidor.

Quando avaliado, o Repetidor irá avaliar a sub expressão repetidamente até que essa sub expressão resulte em valor _verdadeiril_.
Quando isso acontecer o Repetidor para e assume o valor da sub expressão.

Por exemplo, o script a seguir irá inicializar a variável `i` com o valor `0`, então irá repetir um sequenciador que incrementa `i` e testa se `i` é maior que `9`.
Para quando o valor de `i` for `10`:

```
i=0
\(i+=1; i>9)
```

Perceba que para repetir tanto o incremento quanto a comparação, foi necessário colocá-los em um Sequenciador.

Caso o limite seja especificado, o Repetidor irá repetir no máximo essa quantidade de vezes:

```
i=0
\9\(i+=1; 1>100)
```

No script acima o Repetidor não terá sucesso, pois a variável `i` precisa passar de `100`, mas o limite do repetidor é `9`. Neste caso o resultado será NIL.


### Operador Se

A barra vertical `|` entre duas expressões é chamado de Operador Se, e é usado para avaliar a expressão da esquerda somente se a expressão da direita resultar em valor _verdadeiril_. O resultado é NIL caso contrário.

São dois casos de uso:

- Com um nó no lado direito:
  - Neste caso o nó é executado usando o valor do lado esquerdo como argumento.
- Com qualquer outro valor no lado direito:
  - O lado esquerdo será avaliado apenas se a expressão do lado direito for _verdadeiril_.

Normalmente é usado como alternativa para o Seletor, ou em conjunto com o Seletor.

Por exemplo, os seguintes pares de linhas são equivalentes, considere que `func` é uma referência a um nó definido anteriormente:
```
(func:a; a)
a|func

[(func:a; a) b]
[a|func b]
```

### Opcional

O Opcional é denotado por um ponto de interrogação `?` seguido de uma única sub expressão.

O Opcional é usado para ignorar um possível resultado _falsil_ da sub expressão. Fazendo sempre resultar em BOOLEAN Verdadeiro.

Por exemplo, se houver uma expressão dentro de um Sequenciador que pode resultar em _falsil_, mas esse resultado não deve parar o Sequenciador, 
coloca-se a sub expressão dentro de um Opcional:

`(?podeResultarEmFalsil; precisaSerAvaliadoMesmoAssim)`

No script acima, ambas as sub expressões do Sequenciador serão avaliadas, independente do resultado da primeira sub expressão.

### Inversor

O Inversor é denotado pelo simbolo til `~` seguido de um valor.

O Inversor é usado para converter valores _verdadeiril_ em BOOLEAN Falso, e valores _falsil_ em BOOLEAN Verdadeiro.

Em outras palavras, se o valor for NIL ou BOOLEAN Falso, o Inversor terá como resultado BOOLEAN Verdadeiro.
Caso contrário, o resultado será BOOLEAN Falso.

Por exemplo, o script a seguir tem um Inversor seguido de um Sequenciador com uma comparação que resulta em BOOLEAN Falso.
O Inversor então resultará em BOOLEAN Verdadeiro.

`~(1>2)`

Neste caso é necessário usar o Sequenciador para envolver a comparação, pois o Inversor age em cima do valor imediatamente a seguir (que seria o número `1` se não houvesse parêntese). O Sequenciador garante que a comparação seja avaliada primeiro, então o Inversor age em cima do resultado da comparação.

## Referenciando Nós

Em Behaviour, Referenciamento de Nós é equivalente a funções na maioria das linguagens.

Com referenciamento é possível colocar um Nó dentro de uma variável para ser avaliado em outro momento.

Para extrair um nó de uma expressão usa-se o E comercial `&` seguido de uma única sub expressão.

Por exemplo, o script a seguir criará uma referência a um nó que soma o valor de `a` com o valor de `b`:

`soma = &a+b`

Agora a variável `soma` contém uma referência a um nó que soma `a` e `b`.
Para avaliar o nó referenciado usa-se o ponto de exclamação `!` seguido da variável que contém a referência:

`resultado = !soma`

Observe que o nó referenciado espera um valor para as variáveis `a` e `b`, então precisamos defini-las antes de executar o nó:

```
a = 2
b = 3
resultado = !soma
```

Para conveniência pode-se atribuir os valores assim:

`resultado = !soma:a=2,b=3`

Ou assim:

`resultado = !soma:2,3`

O último exemplo funciona por que quando são passados valores para a execução de uma referência, se as variáveis recipientes não forem especificadas,
assume-se que serão as variáveis `a`, `b`, `c`, `d` e `e`, nesta ordem.

Também pode-se omitir o ponto de exclamação se estiver passando valores:

`resultado = soma:2,3`


## Exemplos

### FizzBuzz

O FizzBuzz é um jogo onde os participantes devem contar os números de forma crescente, substituindo os números múltiplos de 3 por fizz, e os números múltiplos de 5 por buzz. Se o número for múltiplo de 3 e 5 ao mesmo tempo, fica fizzbuzz.

Neste exemplo vamos imprimir os resultados do jogo do número 1 até 50:

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

Explicação:

Primeiro inicializa `i` com `1` e entra num Repetidor com Sequenciador. Dentro do Sequenciador, atribui `mod3` com a informação se a variável `i` é divisível por `3`, e `mod5` se `i` é divisível por `5`. Já que essas atribuições podem resultar em BOOLEAN Falso, elas podem parar o Sequenciador, então colocamos cada uma dentro de um Opcional com o ponto de interrogação `?`, que faz o Sequenciador ignorar o resultado das atribuições.

Então o Seletor testa as possibilidades e imprime de acordo.

Por fim, incrementa a variável `i` e testa se `i` é maior que `50`. Quando este teste final resultar em verdadeiro, o Sequenciador irá resultar em verdadeiro e permitirá que o Repetidor pare de repetir.

### Fatorial

O algoritmo de fatorial é um bom exemplo para demonstrar recursão:

```
fatorial = &[
  1 | a<2
  a * fatorial:a-1
]
```

Explicação: 

O Seletor principal tem duas sub expressões. Na primeira, testa se `a` é menor que `2`, caso seja resulta em `1` e pronto.

Na segunda sub expressão, multiplica o valor de `a` pelo resultado da execução de `fatorial` quando chamado com `a-1`.
