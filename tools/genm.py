#
# Generate macro sequence for obtaining number of arguments in list
#
# Required for clang, GCC has a better way
#

LISTLEN = 2048

def main():
    s = '#define FSTR_VA_NARGS_IMPL('
    s += ', '.join(f'_{i}' for i in range(1, LISTLEN+1))
    s += ', N, ...) N'
    print(s)

    s = '#define FSTR_VA_NARGS(...) FSTR_VA_NARGS_IMPL(__VA_ARGS__, '
    s += ', '.join(f'{i}' for i in range(LISTLEN, 0, -1))
    s += ')'
    print(s)


if __name__ == '__main__':
    main()
