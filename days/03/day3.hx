gg
:insert-output cat input.txt
s mul\((\d{1,3}),(\d{1,3})\)<cr>
:yank-join
ge
2]<space>
ge
p
[p
[p
d
<space><S-p>
%
s,<cr>
c<space><esc>
%
| rg -oN '([0-9]+) ([0-9]+)' tmp
| awk 'BEGIN{sum = 0}{sum += $1 * $2} END{print sum}'
<space><S-y>
