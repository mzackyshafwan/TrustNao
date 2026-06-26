# TrustNao
A Game Theory simulator built with C programming language from scratch

About This Project:
TrustNao is my experimental project to simulate Game Theory's games with Liar Game manga influence. I want to explore how decisions, rationality, trust, and deception can be modeled into a system that able to learn with its user.

Technical Highligh:
- Custom Neural Network: The Artificial Neural Network in this program was built on C entirely.
- Flattened Memory Architecture: Instead of using mutiiple array such as w1, w2, w3, and so on, I implement an flattened array, such as w[], so if I want to access specific w, I just need to build a function to get it with indexing.
- Data Persistence: This program use Save Data system using .csv so it doesn't forget the user progress and the model learning progress.

Getting Started:
1. Clone the repo:
   git clone [https://github.com/mzackyshafwan/TrustNao]
2. Compile the code:
   gcc main.c -o trustnao
3. Run the simulation:
   ./trustnao
