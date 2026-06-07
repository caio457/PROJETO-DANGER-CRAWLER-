# 🏰 Dungeon Crawler (Console em C)

## 👨‍💻 Desenvolvedores

* Caio Lima
* Alberto Neto
* Gabriel monducci

---

## 📖 História do Jogo

Você é um aventureiro que chega a uma pequena vila misteriosa. Após conversar com um NPC local, você escolhe sua arma e parte para explorar uma masmorra cheia de perigos.

A dungeon possui três andares, cada um mais desafiador que o outro. Ao longo da jornada, você enfrentará monstros, armadilhas e puzzles. No final, um poderoso **Boss Final** te espera — somente derrotando-o você conseguirá completar sua missão.

---

## 🎮 Como Jogar

### 🎯 Objetivo

Explorar a dungeon, sobreviver aos desafios e derrotar o boss final.

---

### ⌨️ Controles

| Tecla | Ação               |
| ----- | ------------------ |
| `w`   | Move para cima     |
| `a`   | Move para esquerda |
| `s`   | Move para baixo    |
| `d`   | Move para direita  |
| `i`   | Interagir          |
| `o`   | Atacar             |

---

## 🧩 Mecânicas do Jogo

* ❤️ O jogador possui **3 vidas**
* 💀 Perde vida ao:

  * Encostar em espinhos
  * Ser atingido por monstros
* 🔁 Ao perder uma vida, a fase reinicia
* ❌ Ao perder todas as vidas → **Game Over**
* 🏆 Derrotar o boss → **Vitória**

---

## ⚔️ Sistema de Armas

O jogador escolhe uma arma na vila:

### ⚔️ Espada

* Ataque em área (3x2) à frente

### 🏹 Arco e Flecha

* Ataque em linha reta (4 casas)

### 🪄 Cajado

* Ataca todas as 8 posições ao redor

---

## 🗺️ Estrutura do Jogo

### 🏡 Vila (10x10)

* NPC para escolha de arma
* Entrada da dungeon

### 🧱 1º Andar (10x10)

* Tutorial básico
* Chave e porta
* Caixas destruíveis

### ⚠️ 2º Andar (15x15)

* Introdução a:

  * Espinhos
  * Monstro aleatório (X)
  * Botões

### 🔥 3º Andar (25x25)

* Desafio final
* Monstro perseguidor (Y)
* Boss final (Z)

---

## 👾 Inimigos

* **X** → Movimento aleatório
* **Y** → Persegue o jogador
* **Z** → Boss final (comportamento único)

---

## 🔣 Símbolos do Jogo

| Símbolo   | Significado         |
| --------- | ------------------- |
| `< ^ > v` | Jogador             |
| `*`       | Parede              |
| `#`       | Espinho (morte)     |
| `k`       | Caixa destruível    |
| `O`       | Botão               |
| `D`       | Porta fechada       |
| `=`       | Porta aberta        |
| `@`       | Chave               |
| `L`       | Escada              |
| `X`       | Monstro aleatório   |
| `Y`       | Monstro perseguidor |
| `Z`       | Boss final          |

---

## 🧠 Inteligência Artificial

* Monstro X → aleatório
* Monstro Y → segue o jogador
* Boss → comportamento personalizado

---

## ▶️ Como Executar

1. Compile o código:

```bash
gcc main.c -o jogo
```

2. Execute:

```bash
./jogo
```

---

## 🤖 Uso de IA Generativa

Este projeto utilizou IA generativa como ferramenta de apoio para:

* Estruturação do README
* Auxílio na organização do projeto
* Sugestões de melhorias

Todo o código foi compreendido e pode ser explicado pelos desenvolvedores.

---

## 📌 Observações

* Projeto desenvolvido em linguagem **C**
* Interface baseada em **console (ASCII)**
* Foco em lógica de programação e criatividade

---

## ⭐ Objetivo Acadêmico

Este projeto foi desenvolvido com o objetivo de aplicar conceitos de:

* Estruturas de controle
* Matrizes
* Lógica de jogos
* Organização de código

---
