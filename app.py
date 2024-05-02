from collections import defaultdict

class InvertedIndex:
    def __init__(self):
        self.index = defaultdict(set)

    def add_document(self, doc_id, text):
        terms = set(text.split())  # Use set to avoid duplicate terms
        for term in terms:
            self.index[term].add(doc_id)

    def search(self, query):
        terms = query.split()
        
        # Initialize result with the document IDs of the first term
        result = self.index.get(terms[0], set())
        
        # Iterate over the remaining terms and find the intersection of document IDs
        for term in terms[1:]:
            result.intersection_update(self.index.get(term, set()))
        
        return list(result)

# Example usage:
if __name__ == "__main__":
    index = InvertedIndex()
    
    # Adding documents to the index
    index.add_document(1, "apple banana cherry")
    index.add_document(2, "banana date")
    index.add_document(3, "cherry fig grape")
    
    # Searching for documents containing "banana cherry"
    result = index.search("banana")
    print("Documents containing 'banana' and 'cherry':", result)