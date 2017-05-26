<?hh
namespace Usox\Sharesta;

class InputStreamWrapper {

	public static $singleton = null;
	private $index = 0;
	private $data = '';

	public function read($count){
		$length = min($count, strlen($this->data) - $this->index);
		$data = substr($this->data, $this->index);
		$this->index = $this->index + $length;
		return $data;
	}

	public function eof() {
		return $this->index >= strlen($this->data) ? TRUE : FALSE;
	}

	public function write($data){
		$this->index = 0;
		$this->data = $data;
	}

	public function stream_open($path, $mode, $options, &$opened_path){
		return true;
	}

	public function stream_close(){
	}

	public function stream_stat(){
		return array();
	}

	public function stream_flush(){
		return true;
	}

	private function getSingleton() {
		if (static::$singleton === null) {
			static::$singleton = new InputStreamWrapper();
		}
		return static::$singleton;
	}

	public function stream_read($count) {
		return $this->getSingleton()->read($count);
	}

	public function stream_eof() {
		return $this->getSingleton()->eof();
	}

	public function stream_write($data) {
		$this->getSingleton()->write($data);
	}

	public function unlink() {
	}
}

/**
 * Class: RequestBodyTest
 *
 * @see \PHPUnit_Framework_TestCase
 */
class RequestBodyTest extends \PHPUnit_Framework_TestCase {

	public function setUp() {
		stream_wrapper_unregister('php');
		stream_wrapper_register('php', InputStreamWrapper::class);
		$this->request_body = new RequestBody();
	}

	public function testWithoutJsonDataThrowsException() {
		$this->setExpectedException(
			Exception\RequestException::class
		);

		$this->getRequestBody('absolutly-no-json');
	}

	public function testWithEmptyBodyReturnsEmptyMap() {
		$this->assertEquals(
			Map{},
			$this->getRequestBody('')
		);
	}

	public function testWithJsonDataReturnsMap() {
		$request_data = ['data' => 'really'];
		$data = $this->getRequestBody(json_encode($request_data));

		$this->assertEquals(
			new Map($request_data),
			$data
		);
	}

	private function getRequestBody($input) {
		file_put_contents('php://input', $input);
		return $this->request_body->getBody();	
	}

	public function tearDown() {
		stream_wrapper_restore('php');
	}
}
