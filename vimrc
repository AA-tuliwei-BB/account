set sw=4
set ts=4
set mouse=a
set scrolloff=5
set history=1000
set cursorline
set autoindent
set autoread
set number
set ruler

filetype indent on
syntax on

map <c-j> }
map <c-k> {
map : q:a

imap <c-r> <Esc>
imap <c-o> <Esc>o

autocmd BufNewFile *.py 0r /usr/share/vim/vimfiles/template.py
