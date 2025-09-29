project(
  'coach-tyranny-bot',
  'cpp',
  default_options:['default_library=static']
)

inc = include_directories('include')
# you can add dependencies by installing them with 'meson wrap install (package name)'
# then listing them as variables, like 'ex_dep = dependency(example)'
dpp = dependency('dpp')
mpg123 = dependency('mpg123')

executable(
  'coach-tyranny-3',
  ['src/main.cpp', 'src/coach.cpp'], # source files go here
  include_directories: inc,
  install: true, dependencies: [dpp, mpg123] # dependencies go here
)
