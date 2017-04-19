### Prerequesites

I use `clang-3.9`, `cmake-3.5.2` for local development. Travis runs tests with `cmake-2.8.7`, `clang-3.9` and `gcc-4.6.3` (even if I specifiy `gcc-5`). Documentation for this library is generated with [`doxygen-1.8.14`](http://www.stack.nl/~dimitri/doxygen/download.html).

### Build process for Linux

Clone or fork the repo. (cloning in the example)
```bash
$ git clone https://github.com/cirquit/trdrop
$ cd trdrop
```

Build the library:
```bash
$ cd grittr_path && mkdir build
$ cd build && cmake .. && make
```

Or if you want to install the library globally:
```bash
$ cd grittr_path && mkdir build
$ cd build && cmake .. && sudo make install
```

To generate the documentation
```bash
$ cd grittr_path
$ doxygen Doxyfile
$ cd apidocs/html && firefox index.html
```

### Code style

I use `clang-format` with the WebKit style. 

For use in SublimeText:

```bash
sudo apt-get install clang-format
```
Install Clang-Format Package in Sublime Text. `Preferences -> Package Settings -> Clang Format -> Settings (User)`. Copy the following snippet.
```json
{
    "binary": "/usr/bin/clang-format",
    "style": "WebKit",
    "format_on_save": true
}
```

### Code completion

I use the sublime package [C++ Completions](https://github.com/tushortz/CPP-Completions). 