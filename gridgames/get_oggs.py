#!/usr/bin/env python3

from os import makedirs
from sys import argv, stderr, stdout
from time import sleep
from urllib.request import urlopen

import os.path

def usage():
  stdout.write('''\
usage: get_oggs.py word_list out_dir

  For word in word_list, tries to download an ogg audio file from
  http://www.oxforddictionaries.com/
''')

def url_of_word(w):
  fn = '{}__gb_1.ogg'.format(w)
  return 'http://www.oxforddictionaries.com/media/english/uk_pron_ogg/{}/{}/{}/{}'.format(
      fn[:1], fn[:3], fn[:5], fn)

def main():
  if len(argv) != 3:
    usage()
    return
  makedirs(argv[2], exist_ok=True)
  with open(argv[1],'r') as words:
    for w in words.read().split():
      w = w.replace("'","_")
      try:
        sleep(10)
        with open(os.path.join(argv[2],'{}.ogg'.format(w)), 'wb') as wf:
          wf.write(urlopen(url_of_word(w)).read())
      except Exception as e:
        stderr.write('failed to get {} because of {}\n'.format(w, e))

if __name__ == '__main__':
  main()
