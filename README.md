<h1>Distância Levenshtein</h1>
<b>Trabalho da disciplina de Construção de Algoritmos.</b><br>
O algoritmo da distância de Levenshtein calcula quantas operações em uma palavra são necessárias para transformá-la em outra palavra. Exemplo: Para tranformar a palavra "casa" em "carro" são necessárias 2 substituições ('s' em 'r', 'a' em 'r') e a adição da letra 'o'. Portanto, podemos dizer que o custo total foi de 3 alterações (2 substituições e 1 adição). Muitos utilizam também um custo diferente para operações de substituição, que normalmente é 2, pois uma alteração é como uma operação de deletar e inserir, mas aqui utilizarei o mesmo custo (1) para as 3 operações. O custo da substituição pode ser alterado rapidamente pela constante "CUSTO_SUBSTITUICAO".

Função:<br>
<ul>
	<li>Usuário digita a quantidade de palavras similares a serem retornadas.</li>
	<li>Usuário digita uma palavra sem espaço.</li>
	<li>Algoritmo busca no arquivo "palavras.txt" as palavras mais similares e as retorna.</li>
</ul>
