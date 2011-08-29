#!/usr/bin/python
# -*- coding: utf-8 -*-
#
# Created by ZHENG Zhong on 2011-08-29.
#

import os
import re
import sys
from optparse import OptionParser


_IGNORE = [re.compile(exp) for exp in (
    r'^\.git$',
    r'^\.hg$',
    r'^dist$',
    r'^temp$',
    r'^tmp$',
    r'^.+\~$',
    r'^.+\~\..+$',
    r'^.+\.bak$',
    r'^.+\.ccscc$',
    r'^.+\.ncb$',
    r'^.+\.suo$',
    r'^.+\.cmd$',
    r'^.+\.py[co]$',
)]


def is_ignorable(name):
    for pattern in _IGNORE:
        if pattern.match(name):
            return True
    return False


def rename_ns_for_file(input_path, new_name, output_path):
    fin = open(input_path, 'rb')
    try:
        data = fin.read()
    except Exception, exc:
        raise Exception('Fail to read from %s: %s' % (input_path, exc))
    finally:
        fin.close()
    if '\0' not in data:
        # Text file...
        new_data = data.replace('xtree', new_name).replace('XTREE', new_name.upper())
        fout = open(output_path, 'wb')
        try:
            fout.write(new_data)
        except Exception, exc:
            raise Exception('Fail to write to %s: %s' % (output_path, exc))
        finally:
            fout.close()
        print '[RENAME] %s -> %s' % (input_path, output_path)
        return 1
    else:
        # Binary file...
        print '[IGNORE] %s' % input_path
        return 0


def rename_ns_for_dir(input_path, new_name, output_path):
    count = 0
    if not os.path.exists(output_path):
        os.mkdir(output_path)
        print '[ MKDIR] %s -> %s' % (input_path, output_path)
        ++count
    for name in os.listdir(input_path):
        if not is_ignorable(name):
            input_subpath = os.path.join(input_path, name)
            output_subpath = os.path.join(output_path, name.replace('xtree', new_name))
            if os.path.isdir(input_subpath):
                count += rename_ns_for_dir(input_subpath, new_name, output_subpath)
            elif os.path.isfile(input_subpath):
                count += rename_ns_for_file(input_subpath, new_name, output_subpath)
    return count


#---------------------------------------------------------------------------------------------------


def _create_option_parser():
    """
    Creates an option parser to parse command-line arguments.
    """
    parser = OptionParser(usage="""%prog <XTREE_CODEBASE_DIR>
    Rename the xtree project. (use -h or --help for help)""")
    parser.add_option(
        '-n', '--newname',
        dest='new_name',
        metavar='new-name',
        help='specify the new name to replace xtree'
    )
    parser.add_option(
        '-o', '--output',
        dest='output',
        metavar='output-dir',
        help='specify the output directory'
    )
    return parser


def main(argv=None):
    if argv is None:
        argv = sys.argv[1:]
    parser = _create_option_parser()
    (opts, args) = parser.parse_args(argv)
    if not opts.new_name:
        parser.error('The new name is not specified')
    if not opts.output:
        parser.error('The output directory is not specified')
    elif os.path.exists(opts.output):
        parser.error('The output directory "%s" already exists' % opts.output)
    if not args:
        parser.error('The xtree codebase directory is not specified')
    elif len(args) > 1:
        parser.error('Too many xtree codebase directory')
    elif not os.path.isdir(args[0]):
        parser.error('The xtree codebase directory "%s" does not exist' % args[0])
    count = rename_ns_for_dir(args[0], opts.new_name, opts.output)
    print '%d entries converted successfully' % count


if __name__ == '__main__':
    main()


# EOF
