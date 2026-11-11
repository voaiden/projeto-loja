Meu projeto da faculdade (Algoritmos e Pensamento Computacional) sobre unir os conceitos de algoritmos de ordenação com manipulação de arquivos

# 🛒 Sistema de Cadastro de Loja - Atividade Avaliativa 04

Sistema completo desenvolvido em **Linguagem C** que une estruturas de dados, ponteiros, algoritmos de ordenação e manipulação avançada de arquivos de texto para o gerenciamento de um cadastro genérico de produtos de uma loja.

---

## 🚀 Funcionalidades do Sistema

- **Geração Automática de ID:** Identificador incremental gerado automaticamente pelo sistema, sem necessidade de entrada manual.
- **Verificação de Duplicatas:** Impede o cadastro de produtos com o mesmo nome (validação *case-insensitive*).
- **CRUD Completo:**
  - **Criar:** Inserção de novos produtos com validações.
  - **Listar:** Exibição dos dados organizados em formato de tabela formatada.
  - **Buscar:** Pesquisa por nome (suporta busca parcial) e por categoria.
  - **Alterar:** Modificação de dados existentes utilizando arquivo temporário seguro.
  - **Excluir:** Remoção de registros garantindo integridade de dados.
- **Ordenação (Bubble Sort):**
  - Ordenar produtos por nome (A–Z e Z–A).
  - Ordenar produtos por preço (Crescente e Decrescente).
  - *(Exibido via cópia em memória, preservando a ordem original do arquivo).*
- **Segurança de Arquivos:** Utilização da técnica de **arquivo temporário** (`temp.txt`) para evitar corrupção de dados durante alterações e exclusões.
- **Exportação de Dados:** Exportação completa do banco de dados em formato **CSV** (separado por vírgulas).

---

## 📂 Estrutura de Arquivos

O programa interage com os seguintes arquivos locais:
- `produtos.txt`: Base de dados principal onde os registros são salvos permanentemente.
- `temp.txt`: Arquivo auxiliar gerado temporariamente durante operações de escrita segura.
- `produtos_exportado.csv`: Relatório gerado para compatibilidade com softwares de planilha (Excel, Google Sheets).

---

## 🛠️ Como Executar o Projeto

### Pré-requisitos
Você precisará de um compilador de C instalado no seu ambiente (como **GCC** / **MinGW**) ou de um ambiente online (como o **GDB Online**).

### Passos para Compilação e Execução (Via Terminal / GCC):

1. Clone ou baixe os arquivos do projeto (`main.c` e `produtos.txt`) para a mesma pasta.
2. Abra o terminal na pasta do projeto e compile o código:
   ```bash
   gcc main.c -o loja
