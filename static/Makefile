all: style-min.css
	$(MAKE) -C cv all

style-min.css: style.css
	yui-compressor style.css -o style-min.css

clean:
	rm style-min.css
	$(MAKE) -C cv clean

.PHONY: all clean
