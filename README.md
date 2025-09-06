# HTML Static Page Generator
*A simple static page generator that allows you to declare variables directly in your HTML page.*

## INSTRUCTIONS:

### BUILDING:

In order to build this project, you'll need to clone the directory:

```
git clone -j8 https://github.com/samuelnalini/static-page-gen.git
```

Then, go into the project folder and create a build directory
```
cd static-page-gen && mkdir build && cd build
```

Once you're in the build directory, run
```
make
```


### GENERATING YOUR FILE:

The executable is found in **static-page/build/gen**

Run:

`./gen <FILEPATH>`

EXAMPLE:

`./gen ~/Projects/static-page/page/template.html`


This will generate **static.html** in the **same directory as the gen executable** (may change to allow specification later)


### CREATING AND UTILIZING VARIABLES:

Variables can be declared inside or outside of comments, I recommend the following to avoid issues:

```html
<!<--
    @{variable_name} = "hello, world!"
    @{variable_2} = "here I am!"
-->
```

You can then reference variables in your code by wrapping them around curly brackets

`{variable_name} {variable_2}`

will turn into

`hello, world! here I am!`


*You can see an example inside of page/template.html*

### SOME CONSIDERATIONS

*This is one of my first C++ projects and is definitely not intended to be used in large projects. It was mostly just a learning experience for me.*
*Furthermore, this was not built with security in mind, it is rather a learning project.*
*This is only built for linux but can work on other OSes as well with minor tweaks, if not just a rebuild.*

I plan on moving this to https://github.com/samuelnalini/project-frame in the future
