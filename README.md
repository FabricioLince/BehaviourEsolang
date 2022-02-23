# BehaviourExolang
My Exolang called Behaviour

Available to play with at [Replit](https://replit.com/@fabriciorodrigues/bhv)

## The Basics

Behaviour's syntax in inspired in the [Behaviour Tree](https://en.wikipedia.org/wiki/Behavior_tree_(artificial_intelligence,_robotics_and_control))

Every command in Behaviour is an _Expression_.

Every _Expression_ is converted to a _Node_ when parsed.

Every _Node_ can be evaluated.

The following is an example of an Assign Expression, in which the Variable `count` receives the value of `12`:

`count = 12`

To print a value to the console we can use a Print Expression. The Print Expression can be used in the forms:

`@"Hello World!"` will print the string `Hello World!` **without** a trailing newline.

`@@"Hello World!"` will print the string `Hello World!` **with** a trailing newline.

Ending Expressions with semicolon `;` is not necessary, but can be done to organize the code.

## Types

The basic value types in Behaviour are:
- NIL
  - Represents an error or an otherwise empty value
- BOOLEAN
  - True or False
- NUMBER
  - Floating point number
- STRING
  - Regular string of characters
- LIST
  - An array of Variables of any type
- NODE
  - A reference to a Node

A value is _truthy_ when it's anything other than NIL or BOOLEAN False.

A value is _falsy_ when it's NIL or BOOLEAN False.

## Control Nodes

There are Sequencers, Selector and Repeaters, these are the control Nodes.

#### Sequencer

The Sequencer is denoted by a list of zero or more Expressions inside parentheses `()`.

When evaluated the Sequencer will evaluate each of its _child Nodes_ in order, 
this evaluation will stop if:
- A child Node is evaluated falsy,
- Or all children were evaluated truthy.

Then the Sequencer Node is evaluted to the same value of its last child Node value.

In another words, the Sequencer will be succesfull if all of its children are evaluated successfully, in order.

For example, the following Sequencer will evaluate each addition and finally be evaluated to the value of the final addition:

`seq = (2+3; 5+9; 6+13)`

Since none of those addition will cause any problems, by the end, the value of `seq` will be `19`, the value of the last addition in the Sequencer.

An empty Sequencer will be evaluated to NIL.

#### Selector

The Selector is denoted by a list of zero or more Expressions inside square brackets `[]`.

When evaluated the Selector will evaluate each of its _child Nodes_ in order,
this evaluation will stop on the first _child Node_ that is not evaluated falsy.
The Selector assumes the value of the first child that evaluated truthy, or NIL if none of them did.

In another words, the Selector "selects" the value of the first child that evaluated succesfully.

For example, the following Selector will evaluate its children and stop when the second one is evaluated:

`sel = [name; 2+2; 10*3]`

Since `name` is not defined its value is NIL, then the selector evaluates the addition `2+2` and stops. 
By the end, the value of `sel` will be `4`.
The multiplication `10*3` is never evaluated.

An empty Selector will be evaluated to NIL.

#### Repeater

The Repeater is denoted by the back-slash `\` followed by a single Expression.

When evaluated the Repeater will evaluate its child node repeatedly until the child evaluates truthy.
When it happens the Repeater stops and assumes the value BOOLEAN True.

For example, the following script will initialize a Variable `i` with the value `0`, then repeat a Sequencer that increments `i` by `1` and tests if `i` is bigger than `9`.
It will stop when `i` equals `10`:

```
i=0
\(i+=1; i>9)
```

Note that to repeat both the increment and the comparison each time I have to put them inside a Sequencer.

## Node Referencing

in Behaviour, Node Referencing is the equivalent of functions in most languages.

With Node Referencing you can put a Node inside a Variable to be evaluated later.

To extract the Node from an Expression use the ampersand `&` followed by a single Expression.

For example, the following script will create a reference for a Node that adds the values of `a` and `b`:

`sum = &a+b`

To then evaluate the referenced node use the exclamation point `!` followed by the variable that contains the Node:

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

#### Factorial

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
