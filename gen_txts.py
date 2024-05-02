from faker import Faker
import random

# Initialize Faker
fake = Faker()

# Function to generate random English words
def generate_words(num_words):
    words = [fake.word() for _ in range(num_words)]
    return words

# Function to save words to a text file
def save_to_file(words, filename):
    with open(filename, 'w') as file:
        i = 0
        for word in words:
            file.write(word + ' ')
            i += 1
            if i >= 50:
                file.write('\n')
                i = 0

for i in range(100):
    random_words = generate_words(10000)
    save_to_file(random_words, 'zdoc' + str(i + 1) +'.txt')
    print("Archivo zdoc", i, ".txt creado")