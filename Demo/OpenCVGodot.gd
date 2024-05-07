extends OpenCVGodot

@onready var sprite = $Sprite
@onready var sprite2 = $Sprite2
@onready var sprite3 = $Sprite3

var pic:Mat;
var cap:VideoCapture
# Called when the node enters the scene tree for the first time.
func _ready():
	cap = VideoCapture.new()
	cap.open(0,0)
	pic = cap.read()
	var tex: ImageTexture = ImageTexture.create_from_image(pic.get_image())
	
	sprite.texture = tex
	
	##var f = OpenCVGodot.imread("image.png")
	##sprite3.texture = ImageTexture.create_from_image(f.get_image())
	
	print(Mat.new().get_rows(), ", ", Mat.new().get_cols())
	

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	if cap.is_opened():
		var mat = cap.read()
		mat.convert_to(5)
		var p = OpenCVGodot.transpose(mat)
		var tex2: ImageTexture = ImageTexture.create_from_image(p.get_image())
		sprite2.texture = tex2


func _on_button_pressed():
	cap.release()


func _on_open_pressed():
	cap.open(0,0)
