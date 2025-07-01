
import secrets

def generate_hex_string(length):
    return secrets.token_hex(length // 2)

if __name__ == "__main__":
    print(generate_hex_string(64))
