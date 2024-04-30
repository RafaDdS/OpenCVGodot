extends OpenCVGodot

@onready var sprite = $Sprite
@onready var sprite2 = $Sprite2

var pic:Mat;
# Called when the node enters the scene tree for the first time.
func _ready():
	pic = OpenCVGodot.take_picture()
	var tex: ImageTexture = ImageTexture.create_from_image(pic.get_image())
	
	sprite.texture = tex
	


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	var mat = OpenCVGodot.take_picture()
	
	
	var p = OpenCVGodot.sqrt(mat)
	var tex2: ImageTexture = ImageTexture.create_from_image(p.get_image())
	sprite2.texture = tex2
