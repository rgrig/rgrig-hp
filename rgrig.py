import os
import webapp2
import jinja2

jinja_environment = jinja2.Environment(
    loader=jinja2.FileSystemLoader(os.path.dirname(__file__)))

pages = \
  [ ( 'contact', "How to contact RGrig" )
  , ( 'kids', "Games for kids")
  , ( 'papers', "Papers written by RGrig")
  , ( 'programs', "Some programs written by RGrig")
  , ( 'puzzles', "Some puzzles that you can use at conferences")
  , ( 'teaching', "RGrig's teaching activities")
  , ( 'ukc-co527-2016spring', "Operating Systems and Architecture, lecture notes") ]

class MainPage(webapp2.RequestHandler):
  def get(self):
    template_values = dict()
    file_name = '/welcome.html'
    if self.request.path != '/':
      file_name = self.request.path
    file_name = os.path.join(os.path.dirname(__file__),'data'+file_name)
    content_file = open(file_name)
    template_values['content'] = content_file.read()
    content_file.close()
    template_values['title'] = "RGrig's world"
    for p in pages:
      if self.request.path.find(p[0]) != -1:
        template_values['title'] = p[1]
    template = jinja_environment.get_template('index.html')
    self.response.out.write(template.render(template_values))

hooks = [('/', MainPage)]
for p in pages:
  hooks.append(('/'+p[0]+'.html', MainPage))

# TODO: remove this temporary redirect
hooks.append(webapp2.Route('/javats', webapp2.RedirectHandler, defaults={'_uri':'/static/papers/javats.html'}))

app = webapp2.WSGIApplication(hooks, debug=True)
