# User Notes

# Developer Notes

## Build Dependencies

```sh
> git clone git@gitlab.com:m8665/autotools_mvp.git
```

## OSX System Setup

```sh
> brew install \
    autoconf@2.72 \
    automake \
    libtool \
    autoconf-archive \
    pkgconf \
    pkg-config \
    googletest \
    ossp-uuid \
    boost@1.85
> export PKG_CONFIG_PATH=$(which pkg-config) # Add this to the respective rc file.
> echo "export HOMEBREW_PREFIX=$(brew --prefix)" >> ~/.bash_profile
> echo "export LDFLAGS=\"-L/usr/local/opt/boost@1.85/lib -L${HOMEBREW_PREFIX}/lib\"" >> ~/.bash_profile
> echo "export CPPFLAGS=\"-I/usr/local/opt/boost@1.85/include -I${HOMEBREW_PREFIX}/include\"" >> ~/.bash_profile
> echo "export PKG_CONFIG=$(which pkg-config)" >> ~/.bash_profile
```

## Debian Apt System Setup

```sh
> sudo apt install -y \
  build-essential=12.9 \
  autotools-dev=20220109.1 \
  autoconf-archive=20220903-3 \
  checkinstall=1.6.2+git20170426.d24a630-3+b1 \
  zlib1g-dev=1:1.2.13.dfsg-1 \
  libbz2-dev=1.0.8-5+b1 \
  liblzma-dev=5.4.1-0.2 \
  libzstd-dev=1.5.4+dfsg2-5 \
  valgrind=1:3.19.0-1 \
  uuid-dev=2.38.1-5+deb12u3 \
  libgtest-dev=1.12.1-0.2 \
  libgmock-dev=1.12.1-0.2 \
  autoconf-archive=20220903-3 \
  checkinstall=1.6.2+git20170426.d24a630-3+b1 \
  libtool=2.4.7-7~deb12u1 \
  libtool-bin=2.4.7-7~deb12u1 \
  make=4.3-4.1 \
  packaging-dev=0.8.2 \
  libboost-all-dev=1.74.0.3
```

## Build file generation and compiling library and executable Unix Systems

```sh
> autoreconf -i
> rm -rf ./build && mkdir -p ./build
> cd build/ && ../configure
> cd ../
> make -C build
```

## Running Tests

```sh
> make check

// If a test fails you can find that specific test in the build logs and then
// run:
> ./test/gtest_libautotools_mvp --gtest_filter=<name of test>
// example of gtest_filter is:
//   "Test<Class>.<test_name>"

// If you cannot find the logs then you can simply just run:
> ./test/gtest_libautotools_mvp
```

## Notes on Autoformatting Code

First install clang-formatter

### OSX Setup

```sh
> brew install clang-format
```

### Running clang-format
```sh
> ./scripts/clang-format.sh
```
