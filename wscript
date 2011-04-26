from os.path import exists
from shutil import rmtree

def set_options(ctx):
    ctx.tool_options('compiler_cxx')

def clean(cln):
  if exists('build'): rmtree('build')

def configure(ctx):
    ctx.check_tool('compiler_cxx')
    ctx.check_tool('node_addon')

def build(ctx):
    t = ctx.new_task_gen('cxx', 'shlib', 'node_addon')
    t.target = 'hash_ring'
    t.source = [
		'src/md5.cc',
		'src/hash_ring.cc',
		'src/module.cc'
    ]
