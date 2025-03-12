# User Notes

# Developer Notes

## Build Dependencies

```sh
> git clone git@gitlab.com:m8665/autotools_mvp.git
```

## OSX System Setup

### Prerequisites
```sh
> brew install \
    autoconf@2.72 \
    automake \
    libtool \
    autoconf-archive \
    pkgconf \
    pkg-config \
    llvm \
    ossp-uuid
```

```sh
> cd [Path to Where You Want To Build Boost From]
> mkdir boostorg
> cd boostorg
> git clone https://github.com/boostorg/boost.git
> git checkout tags/boost-1.87.0 -b boost-1.87.0
> git submodule update --init --recursive
> ./bootstrap.sh --prefix=/usr/local/boost --with-toolset=clang
> ./b2 clean
> sudo mkdir /usr/local/boost
> sudo ./b2 \
    toolset=clang \
    cxxflags="-stdlib=libc++" \
    linkflags="-stdlib=libc++" \
    --prefix=/usr/local/boost install
> echo "export PKG_CONFIG_PATH=$(which pkg-config)" >> [Your RC file]
> echo "export PKG_CONFIG=$(which pkg-config)" >> [Your RC file]
> echo "export HOMEBREW_PREFIX=$(brew --prefix)" >> ~/.bash_profile
> echo "export LDFLAGS=\"-L[BoostBuild Prefix]/lib -L${HOMEBREW_PREFIX}/lib\"" >> ~/.bash_profile
> echo "export CPPFLAGS=\"-I[BoostBuild Prefix]/include -I${HOMEBREW_PREFIX}/include\"" >> ~/.bash_profile
```

## Build Instruction on Nix Systems

```sh
> autoreconf -i
> rm -rf ./build && mkdir -p ./build
> cd build/ && ../configure
> cd ../
> make -C build
```

## Executing the binary executable to validate

```sh
# Note you can also modify the rpath with the respective tools.
> for i in $(ls /usr/local/boost/lib | grep "\.dylib"); \
do
    ln -s /usr/local/boost/lib/${i} \
        [autotools_mvp Clone Directory]/build/lib/.libs/${i};
done
> ./build/bin/autotools_mvp_runner
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
