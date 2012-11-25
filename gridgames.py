import jinja2
import os
import webapp2

jinja_environment = jinja2.Environment(
    loader=jinja2.FileSystemLoader(
      os.path.join(os.path.dirname(__file__), 'gridgames')))

class GamePage(webapp2.RequestHandler):
  def get(self):
    file_name = self.request.path[len('/gridgames/'):]
    if file_name == '': file_name = 'index.html'
    template_values = dict()
    for arg in self.request.arguments():
      template_values[arg] = self.request.get(arg)
    template = jinja_environment.get_template(file_name)
    self.response.out.write(template.render(template_values))

hooks = [('/gridgames', GamePage), ('/gridgames/.*.html', GamePage)]
app = webapp2.WSGIApplication(hooks, debug=True)
