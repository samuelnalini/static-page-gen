<b>HTML Static Page Generator</b>

<i>A simple static page generator that allows you to declare variables directly in your HTML page.</i>

This is one of my first C++ projects and is definitely not intended to be used in large projects. It was mostly just a learning experience for me.

Variables can be declared inside of comments, I recommend the following:

```html
<!<--
    @{variable_name} = "hello, world!"
    @{variable_2} = "here I am!"
-->
```

You can then reference variables in your code by wrapping them around curly brackets

```html
{variable_name} {variable_2}
```

will turn into

```html
    hello, world! here I am!
```
