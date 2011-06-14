#!/usr/bin/python
# -*- coding: utf-8 -*-
#
# Created by ZHENG Zhong on 2011-02-24.
#

import os
import sys
from optparse import OptionParser


_EXT_IGNORES = ('.pyc', '.pyo', '.jpg', '.png', '.gif', '.exe', '.i',)


_EOL_FORMATS = {
    'win': '\r\n', 'crlf': '\r\n',
    'unix': '\n', 'lf': '\n',
    'mac': '\r', 'cr': '\r',
}


class EndOfLineError(Exception):
    pass


def fixeol_file(file_path, eol, auto_convert):
    fin = open(file_path, 'rb')
    try:
        data = fin.read()
    except Exception, exc:
        raise EndOfLineError('Fail to read from %s: %s' % (file_path, exc))
    finally:
        fin.close()
    new_data = data
    if '\0' not in data:
        new_data = data.replace('\r\n', '\n').replace('\r', '\n').replace('\n', eol)
    if new_data == data:
        print '[ ]', file_path  # [ ] : Untouched.
    elif auto_convert:
        fout = open(file_path, 'wb')
        try:
            fout.write(new_data)
        except Exception, exc:
            raise EndOfLineError('Fail to write to %s: %s' % (file_path, exc))
        finally:
            fout.close()
        print '[C]', file_path  # [C] : Converted.
    else:
        print '[X]', file_path  # [X] : Detected (but not converted).


def fixeol(file_or_dir, eol, auto_convert):
    # Retrieve a list of files to check/fix.
    file_or_dir = os.path.join(os.getcwd(), file_or_dir)
    file_list = []
    if os.path.isfile(file_or_dir):
        file_list.append(file_or_dir)
    elif os.path.isdir(file_or_dir):
        for dir_path, dir_names, file_names in os.walk(file_or_dir):
            file_list.extend([os.path.join(dir_path, n) for n in file_names])
    else:
        raise EndOfLineError('%s is neither a directory nor a file.' % file_or_dir)
    # Check and fix end-of-line for each file.
    for file_path in file_list:
        ext = os.path.splitext(file_path)[1].lower()
        if ext not in _EXT_IGNORES:
            try:
                fixeol_file(file_path, eol, auto_convert)
            except Exception, exc:
                print str(exc)


#---------------------------------------------------------------------------------------------------


def _create_option_parser():
    """
    Creates an option parser to parse command-line arguments.
    """
    parser = OptionParser(usage="""%prog <INPUT_FILE_OR_DIR>
    Fix end-of-line characters (use -h or --help for help)""")
    parser.add_option(
        '-e', '--eol',
        dest='eol',
        metavar='eol-format',
        help='specify the desired end-of-line format (win/CRLF, unix/LF, mac/CR)'
    )
    parser.add_option(
        '--auto-convert',
        dest='auto_convert',
        default=False,
        action='store_true',
        help='specify whether to auto-convert end-of-line (disabled by default)'
    )
    return parser


def main(argv=None):
    if argv is None:
        argv = sys.argv[1:]
    parser = _create_option_parser()
    (opts, args) = parser.parse_args(argv)
    if not opts.eol:
        parser.error('The desired end-of-line format is not specified')
    elif opts.eol.lower() not in _EOL_FORMATS:
        parser.error('Unknown end-of-line format')
    elif not args:
        parser.error('The input file or directory is not specified')
    elif len(args) > 1:
        parser.error('Too many input file or directory arguments')
    fixeol(args[0], _EOL_FORMATS[opts.eol.lower()], opts.auto_convert)


if __name__ == '__main__':
    main()


# EOF
