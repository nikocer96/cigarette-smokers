# cigarette-smokers
# Cigarette Smokers Problem Solver

Questo progetto è una soluzione per il problema dei fumatori di sigarette (Cigarette Smokers Problem), un classico problema di sincronizzazione presentato da Suhas Patil nel 1971. Il problema mette in luce la necessità di coordinare l'accesso alle risorse condivise tra diversi processi in un ambiente multi-threaded.

## Problema

Il problema coinvolge tre fumatori di sigarette e un agente. Ogni fumatore ha una fornitura infinita di un solo ingrediente: tabacco, cartine o accendini. L'agente mette a disposizione due ingredienti alla volta, e il fumatore che ha il terzo ingrediente può fumare una sigaretta. L'obiettivo è garantire che l'agente e i fumatori lavorino in modo sincronizzato, senza creare condizioni di deadlock o race condition.

## Soluzione

La soluzione proposta utilizza la programmazione concorrente e semafori per gestire la sincronizzazione tra i processi. L'agente mette gli ingredienti a disposizione e segnala ai fumatori, che aspettano il loro turno per fumare. I semafori garantiscono l'accesso esclusivo alle risorse condivise e prevengono problemi di sincronizzazione.

