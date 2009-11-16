import os
from google.appengine.ext import webapp
from google.appengine.ext.webapp import template
from google.appengine.ext.webapp.util import run_wsgi_app

pages = [('papers', "Papers written by RGrig"),
        ('teaching', "RGrig's teaching activities"),
        ('puzzles', "Some puzzles that you can use at conferences"),
        ('programs', "Some programs written by RGrig")]

class MainPage(webapp.RequestHandler):
  def get(self):
    template_values = dict()
    file_name = '/welcome.html'
    if self.request.path != '/': file_name = self.request.path
    file_name = os.path.join(os.path.dirname(__file__),'data'+file_name)
    content_file = open(file_name)
    template_values['content'] = content_file.read()
    content_file.close()
    template_values['title'] = "RGrig's world"
    for p in pages:
      if self.request.path.find(p[0]) != -1:
        template_values['title'] = p[1]
    path = os.path.join(os.path.dirname(__file__), 'index.html')
    self.response.out.write(template.render(path, template_values))

hooks = [('/', MainPage)]
for p in pages: hooks.append(('/'+p[0]+'.html', MainPage))
application = webapp.WSGIApplication(hooks, debug=True)

def main():
  run_wsgi_app(application)

if __name__ == "__main__":
  main()
