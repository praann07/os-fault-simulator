from flask import Flask
app = Flask(__name__)
@app.get('/')
def root():
    return 'Mini OK'
if __name__ == '__main__':
    app.run(host='127.0.0.1', port=5001, debug=False)
