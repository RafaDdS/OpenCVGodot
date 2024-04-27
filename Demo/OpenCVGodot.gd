extends OpenCVGodot

@onready var sprite = $Sprite
@onready var sprite2 = $Sprite2

var pic:Mat;
# Called when the node enters the scene tree for the first time.
func _ready():
	pic = OpenCVGodot.take_picture()
	var tex: ImageTexture = ImageTexture.create_from_image(pic.get_image())
	
	
	sprite.texture = tex
	
	var mat = Mat.new()
	var tex2: ImageTexture = ImageTexture.create_from_image(mat.get_image())
	sprite2.texture = tex2
	
	print(mat.get_image())
	


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	var mat = OpenCVGodot.take_picture()
	
	
	var p = OpenCVGodot.subtract(mat, pic)
	var tex2: ImageTexture = ImageTexture.create_from_image(p.get_image())
	sprite2.texture = tex2
