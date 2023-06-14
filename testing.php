<?

class TestingClass {

    /* Some Block Comment */
    // Some Line Comment
    /*
     * Some Block Comment
     */

    $something = false;
    ?int $somethingElse = 0;
    private ?string $name = null;
    public ?string $name2 = null;
    protected ?string $name3 = null;
    public static ?string $name4 = null;

    public function __construct() {
        echo "TestingClass";
    }

}

?>
